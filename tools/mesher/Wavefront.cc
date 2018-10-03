#include "Wavefront.hh"
#include <fstream>
#include <cstring>


struct WavefrontContext
{
    uint32_t vertexCount = 0;
    uint32_t uvCount = 0;
    uint32_t normalCount = 0;
    uint32_t faceCount = 0;
    WavefrontMaterial *currentMaterial;
    WavefrontObject *currentObject;
};


static void parseMaterials(
    const std::vector<std::string> &content,
    WavefrontModel &model,
    void *context )
{
    WavefrontContext &ctx = *((WavefrontContext*)context);

    if (content[0] == "newmtl" && content.size() == 2)
    {
        ctx.currentMaterial = new WavefrontMaterial(content[1]);
        model.materialLibrary.insert(std::pair<std::string,WavefrontMaterial*>(content[1], ctx.currentMaterial));
        std::cout << "Found material '" << content[1] << std::endl;
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


static void parseModel(
    const std::vector<std::string> &content,
    WavefrontModel &model,
    void *context )
{
    WavefrontContext &ctx = *((WavefrontContext*)context);

    if (content[0] == "mtllib" && content.size() == 2)
    {
        ctx.currentMaterial = nullptr;
        model.parseFile(content[1], parseMaterials, context);
        ctx.currentMaterial = nullptr;
    }
    else
    if (content[0] == "o" && content.size() == 2)
    {
        ctx.currentObject = new WavefrontObject(content[1]);
        model.objects.push_back(ctx.currentObject);
    }

    if (ctx.currentObject == nullptr) return;

    // handles vertices
    if (content[0] == "v" && (content.size() == 4 || content.size() == 5))
    {
        Vector3f vertex;
        vertex.x = (float) atof(content[1].c_str());
        vertex.y = (float) atof(content[2].c_str());
        vertex.z = (float) atof(content[3].c_str());
        /*if (content.size() == 5)
            vertex.w = atof(content[4].c_str());
        else
            vertex.w = 1.0F;*/
        ctx.currentObject->vertices.push_back(vertex);
        ++ctx.vertexCount;
    }
    else
    // handles textures coordinates
    if (content[0] == "usemtl" && content.size() == 2)
    {
        ctx.currentMaterial = nullptr;
        auto it = model.materialLibrary.find(content[1]);
        if (it != model.materialLibrary.end())
            ctx.currentMaterial = it->second;
        ctx.currentObject->material = content[1];
    }
    else
    // handles normals
    if (content[0] == "vn" && content.size() == 4)
    {
        Vector3f normal;
        normal.x = (float) atof(content[1].c_str());
        normal.y = (float) atof(content[2].c_str());
        normal.z = (float) atof(content[3].c_str());
        ctx.currentObject->normals.push_back(normal);
        ++ctx.normalCount;
    }
    else
    // handles normals
    if (content[0] == "vt" && content.size() == 3)
    {
        Vector2f uv;
        uv.x = (float) atof(content[1].c_str());
        uv.y = (float) atof(content[2].c_str());
        ctx.currentObject->uvs.push_back(uv);
        ++ctx.uvCount;
    }
    else
    // handles faces
    if (content[0] == "f" && content.size() == 4)
    {
        Vector3u first  = model.getTriple(content[1]);
        Vector3u second = model.getTriple(content[2]);
        Vector3u third  = model.getTriple(content[3]);

        WavefrontFace face;
        face.vertices[0] = first.x;
        face.vertices[1] = second.x;
        face.vertices[2] = third.x;
        face.uvs[0] = first.y;
        face.uvs[1] = second.y;
        face.uvs[2] = third.y;
        face.normals[0] = first.z;
        face.normals[1] = second.z;
        face.normals[2] = third.z;

        ctx.currentObject->faces.push_back(face);
        ++ctx.faceCount;
    }
}


WavefrontObject::WavefrontObject(
    const std::string &name ) : name(name)
{
}


WavefrontObject::WavefrontObject(
    WavefrontObject&& object )
{
    name = object.name;
	material = object.material;
	vertices.swap(object.vertices);
    normals.swap(object.normals);
    uvs.swap(object.uvs);
}


WavefrontModel::WavefrontModel(
    WavefrontModel&& model )
{
    objects.swap(model.objects);
}


char *WavefrontModel::cleanup( char *line )
{
    // ignore spaces from the begin
    char *ptr = line;
    while (*ptr == ' ' || *ptr == '\t') ++ptr;
    if (*ptr == 0) return nullptr;
    // remove line control characters from the end
    size_t len = strlen(ptr);
    if (len == 0) return nullptr;
    for (int i = (int) len - 1; i >= 0 && ptr[i] <= ' '; --i) ptr[i] = 0;

//std::cout << "Line: '" << ptr << "'" << std::endl;
    if (*ptr == 0 || *ptr == '#') return nullptr;

    return ptr;
}


void WavefrontModel::split(
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


void WavefrontModel::parseFile(
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


void WavefrontModel::parseFile(
    std::istream &in,
    WavefrontParser *parser,
    void *context )
{
    if (in.good() == false)
        throw std::string("Unable to read data from Wavefront input");

    char line[512];
    std::vector<std::string> content;

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


Vector3u WavefrontModel::getTriple(
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
        /* fallthrough */
        case 2: current.y = (uint32_t) atoi(values[1].c_str());
        /* fallthrough */
        case 1: current.x = (uint32_t) atoi(values[0].c_str());
        /* fallthrough */
        default: break;
    }
//std::cout << "    Vector: '" << current.x << ", " << current.y << ", " << current.z << "'" << std::endl;
    return current;
}


WavefrontModel WavefrontModel::load(
    const std::string &fileName )
{
    WavefrontContext context;
    WavefrontModel model;
    model.parseFile(fileName, parseModel, &context);
    model.vertexCount = context.vertexCount;
    model.uvCount     = context.uvCount;
    model.normalCount = context.normalCount;
    model.faceCount   = context.faceCount;
    return model;
}

