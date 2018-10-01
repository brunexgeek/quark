#include "Wavefront.hh"
#include <fstream>
#include <cstring>


struct WavefrontContext
{
    uint32_t indexCounter = 0;
    uint32_t materialCounter = 0;
    std::vector<int> materials;
    std::vector<Vector3f> vertices;
    std::vector<Vector3f> normals;
    std::vector<Vector2f> uvs;
    WavefrontMaterial *currentMaterial;
};


static void parseMaterials(
    const std::vector<std::string> &content,
    WavefrontObject &object,
    void *context )
{
    WavefrontContext &ctx = *((WavefrontContext*)context);

    if (content[0] == "newmtl" && content.size() == 2)
    {
        ctx.currentMaterial = new WavefrontMaterial(ctx.materialCounter++, content[1]);
        object.materialLibrary.insert(std::pair<std::string,WavefrontMaterial*>(content[1], ctx.currentMaterial));
        std::cout << "Found material '" << content[1] << "'  [#" << ctx.currentMaterial->id << "]" << std::endl;
    }
    else
    if (content[0] == "Kd" && content.size() == 4 && ctx.currentMaterial != NULL)
    {
        ctx.currentMaterial->colorDiffuse.x = (float) atof(content[1].c_str());
        ctx.currentMaterial->colorDiffuse.y = (float) atof(content[2].c_str());
        ctx.currentMaterial->colorDiffuse.z = (float) atof(content[3].c_str());
    }
    else
    if (content[0] == "map_Kd" && content.size() == 2 && ctx.currentMaterial != NULL)
    {
        ctx.currentMaterial->texture = content[1];
    }
}


static void parseWavefront(
    const std::vector<std::string> &content,
    WavefrontObject &object,
    void *context )
{
    WavefrontContext &ctx = *((WavefrontContext*)context);

    bool foundMaterials = false;
    if (content[0] == "mtllib" && content.size() == 2 && !foundMaterials)
    {
        ctx.currentMaterial = nullptr;
        object.parseFile(content[1], parseMaterials, context);
        ctx.currentMaterial = nullptr;
        foundMaterials = true;
    }
    else
    // handles vertices
    if (content[0] == "v" && (content.size() == 4 || content.size() == 5))
    {
        Vector3f vertex;
        vertex.x = atof(content[1].c_str());
        vertex.y = atof(content[2].c_str());
        vertex.z = atof(content[3].c_str());
        /*if (content.size() == 5)
            vertex.w = atof(content[4].c_str());
        else
            vertex.w = 1.0F;*/
        ctx.vertices.push_back(vertex);
    }
    else
    // handles textures coordinates
    if (content[0] == "usemtl" && content.size() == 2)
    {
        ctx.currentMaterial = nullptr;
        auto it = object.materialLibrary.find(content[1]);
        if (it != object.materialLibrary.end())
            ctx.currentMaterial = it->second;
    }
    else
    // handles normals
    if (content[0] == "vn" && content.size() == 4)
    {
        Vector3f normal;
        normal.x = atof(content[1].c_str());
        normal.y = atof(content[2].c_str());
        normal.z = atof(content[3].c_str());
        ctx.normals.push_back(normal);
    }
    else
    // handles normals
    if (content[0] == "vt" && content.size() == 3)
    {
        Vector2f uv;
        uv.x = atof(content[1].c_str());
        uv.y = atof(content[2].c_str());
        ctx.uvs.push_back(uv);
    }
    else
    // handles faces
    if (content[0] == "f" && content.size() == 4)
    {
        Vector3u first  = object.getTriple(content[1]);
        Vector3u second = object.getTriple(content[2]);
        Vector3u third  = object.getTriple(content[3]);

        object.createEntry(ctx.indexCounter++, object.faces, ctx.vertices, first.x - 1,  ctx.uvs, first.y - 1,  ctx.normals, first.z - 1);
        object.createEntry(ctx.indexCounter++, object.faces, ctx.vertices, second.x - 1, ctx.uvs, second.y - 1, ctx.normals, second.z - 1);
        object.createEntry(ctx.indexCounter++, object.faces, ctx.vertices, third.x - 1,  ctx.uvs, third.y - 1,  ctx.normals, third.z - 1);

        //faceIndex.push_back(Vector3u(first.x - 1, second.x - 1, third.x - 1));
        //normalIndex.push_back(Vector3u(first.z - 1, second.z - 1, third.z - 1));
    }
}



WavefrontObject::WavefrontObject( WavefrontObject&& object )
{
    vertexCount = object.vertexCount;
	normalCount = object.normalCount;
	materialLibrary.swap(object.materialLibrary);
	faces.swap(object.faces);
}


char *WavefrontObject::cleanup( char *line )
{
    // ignore spaces from the begin
    char *ptr = line;
    while (*ptr == ' ' || *ptr == '\t') ++ptr;
    if (*ptr == 0) return nullptr;
    // remove line control characters from the end
    size_t len = strlen(ptr);
    if (len == 0) return nullptr;
    for (size_t i = len - 1; i >= 0 && ptr[i] <= ' '; --i) ptr[i] = 0;

//std::cout << "Line: '" << ptr << "'" << std::endl;
    if (*ptr == 0 || *ptr == '#') return nullptr;

    return ptr;
}


void WavefrontObject::split(
    char *text,
    std::vector<std::string> &words,
    const char separator )
{

    char *ptr = text;
    char *end = text;

    while (*end != 0)
    {
        // remove leading whitespaces
        while (*ptr == ' ' || *ptr == '\t') end = ++ptr;
        // look for the next whitespace
        //while (*end != ' ' && *end != '\t' && *end != 0) ++end;
        while (*end != separator && *end != 0) ++end;

        if (*end == 0)
        {
            if (*ptr == 0) break;
            words.push_back(ptr);
//std::cout << "   '" << ptr << "'" << std::endl;
            break;
        }
        else
        {
            *end = 0;
//std::cout << "   '" << ptr << "'" << std::endl;
            words.push_back(ptr);
            ptr = end = end + 1;
        }
    }
}


void WavefrontObject::parseFile(
    const std::string &fileName,
    WavefrontParser *parser,
    void *context )
{
    std::ifstream input(fileName.c_str(), std::ios_base::in);
    if (input.good())
    {
        parseFile(input, parser, context);
        input.close();
    }
}


void WavefrontObject::parseFile(
    std::istream &in,
    WavefrontParser *parser,
    void *context )
{
    if (in.good() == false)
        throw std::string("Unable to read data from Wavefront input");

    char line[512];
    std::vector<std::string> content;
    int currentMaterial = -1;

    while (in.good())
    {
        line[0] = 0;
        in.getline(line, sizeof(line) - 1);

        // cleanup and split
        char *ptr = cleanup(line);
        if (ptr == nullptr) continue;
        content.clear();
        split(ptr, content);
        if (content.size() == 0) continue;

        // parse the content
        parser(content, *this, context);
    }
}


Vector3u WavefrontObject::getTriple(
    const std::string &triple )
{
    std::vector<std::string> values;
    char temp[512];
    strncpy(temp, triple.c_str(), sizeof(temp)-1);
//std::cout << "  Face: '" << triple << "'" << std::endl;
    split(temp, values, '/');
    Vector3u current = { 0, 0, 0 };
    switch (values.size())
    {
        case 3: current.z = (uint32_t) atoi(values[2].c_str());
        case 2: current.y = (uint32_t) atoi(values[1].c_str());
        case 1: current.x = (uint32_t) atoi(values[0].c_str());
        default: break;
    }
//std::cout << "    Vector: '" << current.x << ", " << current.y << ", " << current.z << "'" << std::endl;
    return current;
}


WavefrontObject WavefrontObject::load(
    const std::string &fileName )
{
    WavefrontContext context;
    WavefrontObject object;
    object.parseFile(fileName, parseWavefront, &context);
    object.vertexCount = context.vertices.size();
    object.normalCount = context.normals.size();
    object.uvCount = context.uvs.size();
    return object;
}


void WavefrontObject::createEntry(
    uint32_t index,
    std::list<WavefrontVertex> &out,
    const std::vector<Vector3f> &vertices,
    const uint32_t vertexIndex,
    const std::vector<Vector2f> &uvs,
    const uint32_t uvIndex,
    const std::vector<Vector3f> &normals,
    const uint32_t normalIndex )
{
    WavefrontVertex current;
    //current.index = index;
    current.vertex = vertices[vertexIndex];
    current.uv      = uvs[uvIndex];
    current.normal = normals[normalIndex];
    out.push_back(current);
}
