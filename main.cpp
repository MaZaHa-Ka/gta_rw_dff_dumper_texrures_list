// MIT лицензи€ by MAZAHAKA!!
#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "Windows.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdint>
#include <vector>
#include <cstdlib> // дл€ system("cls")

#include "tools.h"
#define ROUNDSIZE(x) if((x) & 3) (x) += 4 - ((x)&3)
#define null NULL

// anims :(
//#include "../plugin_III.h"
//#include "AnimBlendFrameData.h"
//#include "CAnimationStyleDescriptor.h"
//#include "CAnimBlendAssocGroup.h"
//#include "CAnimBlendAssociation.h"
//#include "CAnimBlendClumpData.h"
//#include "CAnimBlendHierarchy.h"
//#include "CAnimBlendLink.h"
//#include "CAnimBlendNode.h"
//#include "CAnimBlendSequence.h"
//#include "CAnimBlock.h"
//#include "CAnimManager.h"


//------------------------------------------------------------------
HANDLE InitConsole() // with proto
{
    AllocConsole();

    //SetConsoleOutputCP(866);
    setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);


    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);

    return hConsole;
}

//void LeaveConsole(HANDLE hConsole = nullptr)
void LeaveConsole(HANDLE hConsole) // with proto
{
    if (hConsole != nullptr) { SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); } // Reset to default color
    FreeConsole();
}

void SetCurrDir(std::string path = "")
{
    if (path != "") { SetCurrentDirectory(path.c_str()); return; }
    char currentDir[MAX_PATH]; // dynamic set curr dir to exe
    GetModuleFileName(NULL, currentDir, MAX_PATH);
    std::string::size_type pos = std::string(currentDir).find_last_of("\\/");
    SetCurrentDirectory(std::string(currentDir).substr(0, pos).c_str());
}

std::vector<std::string> LoadAllFilesFromFolder(std::string folderPath, std::string ext)
{
    if (folderPath.back() != '\\' && folderPath.back() != '/') { folderPath += "\\"; }

    std::vector<std::string> oggFiles; // —оздаем вектор дл€ хранени€ путей к .ogg файлам
    WIN32_FIND_DATA findFileData;
    //HANDLE hFind = FindFirstFile((folderPath + "*.ogg").c_str(), &findFileData);
    HANDLE hFind = FindFirstFile((folderPath + "*." + ext).c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            std::string filePath = folderPath + findFileData.cFileName;
            oggFiles.push_back(filePath);
            //std::cout << "ScanFile: " << filePath << "\n";
        } while (FindNextFile(hFind, &findFileData) != 0);

        FindClose(hFind);
    }

    return oggFiles;
}



class CAnimBlendHierarchy
{
public:
    char name[24];
    void* sequences;
    int16_t numSequences;
    bool compressed;
    bool keepCompressed;
    float totalLength;
    void* linkPtr;
}; // 40байт

#define NUMANIMBLOCKS 60
#define MAX_ANIMBLOCK_NAME 20

struct CAnimBlock
{
    char name[MAX_ANIMBLOCK_NAME];
    bool isLoaded;
    int16_t refCount;
    int32_t firstIndex;	// first animtion in ms_aAnimations
    int32_t numAnims;
};

struct IfpHeader {
    char ident[4];
    uint32_t size;
};

void LoadIFP(const char* path)
{
    IfpHeader anpk, info, name, dgan, cpan, anim;
#define ROUNDSIZE(x) if((x) & 3) (x) += 4 - ((x)&3)
    std::vector<CAnimBlendHierarchy> animations;
    //CAnimBlock ms_aAnimBlocks[NUMANIMBLOCKS];
    CAnimBlock ped_block;
    int dummy = 0;


    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) { std::cout << "Error opening file: " << path << std::endl; return; }

    file.read((char*)(&anpk), sizeof(anpk));
    ROUNDSIZE(anpk.size);

    file.read((char*)(&info), sizeof(info));
    ROUNDSIZE(info.size);


    //----some info AnimBlock
    //uint32_t numAnimations;
    file.read((char*)(&ped_block.numAnims), sizeof(ped_block.numAnims));

    char block_name[4]; // ped\0
    file.read((char*)(&block_name), sizeof(block_name));
    strncpy(ped_block.name, block_name, MAX_ANIMBLOCK_NAME);

    ped_block.firstIndex = 0; // ms_numAnimations debug 0
    ped_block.isLoaded = true;
    //---------------

    //animations.resize(ped_block.numAnims);
    for (int j = 0; j < ped_block.numAnims; j++)
    {
        CAnimBlendHierarchy* her = new CAnimBlendHierarchy();

        // animation name
        file.read((char*)(&name), sizeof(IfpHeader)); // name tmp перечисл€ем
        ROUNDSIZE(name.size);
        file.read((char*)(&her->name), name.size);
        //strncpy(this->name, name, 24); // CAnimBlendHierarchy::SetName(char* name)

        file.read((char*)(&dgan), sizeof(IfpHeader));
        ROUNDSIZE(dgan.size);
        file.read((char*)(&info), sizeof(IfpHeader));
        ROUNDSIZE(info.size);
        //file.read((char*)(&her->numSequences), sizeof(her->numSequences)); //22
        file.read((char*)(&her->numSequences), sizeof(int)); //22 pad2
        //her->numSequences = *(int*)buf;
        file.read((char*)(&dummy), sizeof(dummy)); //42 unk
        //file.seekg(her->numSequences * (100 + 8), std::ios::cur); // 100 anim+kr00 не все 100
        //continue;

        for (int k = 0; k < her->numSequences; k++) {
            file.read((char*)(&cpan), sizeof(IfpHeader));
            ROUNDSIZE(cpan.size); // dgan?
            file.seekg(cpan.size, std::ios::cur); // 100 anim+kr00
            //std::cout << "cpan sz: " << cpan.size << "\n";
            continue;

            file.read((char*)(&anim), sizeof(IfpHeader));
            ROUNDSIZE(anim.size);
            //RwStreamRead(stream, buf, anim.size);
            //int numFrames = *(int*)(buf + 28);
            file.seekg(anim.size, std::ios::cur); // 44
        }

        //VALIDATE_SIZE(CAnimBlendSequence, 0x30);
        //file.seekg(her->numSequences * 0x30, std::ios::cur); // 24 уже считали


        animations.push_back(*her);
    }

    //file.seekg(16, std::ios::cur);
    //file.seekg(sizeof(IFPHeader), std::ios::beg);
    //file.read(reinterpret_cast<char*>(&numAnimations), sizeof(numAnimations));

    for (int i = 0; i < animations.size(); i++)
    {
        std::cout << animations[i].name << "\n";
    }
}


void func()
{
    const char* ifp = "D:\\games\\BONUS\\IFP\\anims\\anim_vc\\ped.ifp";
    LoadIFP(ifp);
}






enum RwCorePluginID // PluginID = mac(vendor + RwCorePluginID)
{
    rwID_NAOBJECT = 0x00,
    rwID_STRUCT = 0x01,
    rwID_STRING = 0x02,
    rwID_EXTENSION = 0x03,
    rwID_CAMERA = 0x05,
    rwID_TEXTURE = 0x06,
    rwID_MATERIAL = 0x07,
    rwID_MATLIST = 0x08,
    rwID_ATOMICSECT = 0x09,
    rwID_PLANESECT = 0x0A,
    rwID_WORLD = 0x0B,
    rwID_SPLINE = 0x0C,
    rwID_MATRIX = 0x0D,
    rwID_FRAMELIST = 0x0E,
    rwID_GEOMETRY = 0x0F,
    rwID_CLUMP = 0x10,
    rwID_LIGHT = 0x12,
    rwID_UNICODESTRING = 0x13,
    rwID_ATOMIC = 0x14,
    rwID_TEXTURENATIVE = 0x15,
    rwID_TEXDICTIONARY = 0x16,
    rwID_ANIMDATABASE = 0x17,
    rwID_IMAGE = 0x18,
    rwID_SKINANIMATION = 0x19,
    rwID_GEOMETRYLIST = 0x1A,
    rwID_HANIMANIMATION = 0x1B,
    rwID_TEAM = 0x1C,
    rwID_CROWD = 0x1D,
    rwID_DMORPHANIMATION = 0x1E,
    rwID_RIGHTTORENDER = 0x1f,
    rwID_MTEFFECTNATIVE = 0x20,
    rwID_MTEFFECTDICT = 0x21,
    rwID_TEAMDICTIONARY = 0x22,
    rwID_PITEXDICTIONARY = 0x23,
    rwID_TOC = 0x24,
    rwID_PRTSTDGLOBALDATA = 0x25,
    /* Insert before MAX and increment MAX */
    rwID_COREPLUGINIDMAX = 0x26,
};

// rwbase.h
#define MAKEPLUGINID(v, id) (((v & 0xFFFFFF) << 8) | (id & 0xFF))
#define MAKEPIPEID(v, id) (((v & 0xFFFF) << 16) | (id & 0xFFFF))
enum VendorID
{
    VEND_CORE = 0,
    VEND_CRITERIONTK = 1,
    VEND_CRITERIONINT = 4,
    VEND_CRITERIONWORLD = 5,
    // Used for rasters (platform-specific)
    VEND_RASTER = 10,
    // Used for driver/device allocation tags
    VEND_DRIVER = 11
};
enum PluginID // maybe RwCorePluginID hardcode \/
{
    // Core
    ID_NAOBJECT = MAKEPLUGINID(VEND_CORE, 0x00),
    ID_STRUCT = MAKEPLUGINID(VEND_CORE, 0x01),
    ID_STRING = MAKEPLUGINID(VEND_CORE, 0x02),
    ID_EXTENSION = MAKEPLUGINID(VEND_CORE, 0x03),
    ID_CAMERA = MAKEPLUGINID(VEND_CORE, 0x05),
    ID_TEXTURE = MAKEPLUGINID(VEND_CORE, 0x06),
    ID_MATERIAL = MAKEPLUGINID(VEND_CORE, 0x07),
    ID_MATLIST = MAKEPLUGINID(VEND_CORE, 0x08),
    ID_WORLD = MAKEPLUGINID(VEND_CORE, 0x0B),
    ID_MATRIX = MAKEPLUGINID(VEND_CORE, 0x0D),
    ID_FRAMELIST = MAKEPLUGINID(VEND_CORE, 0x0E),
    ID_GEOMETRY = MAKEPLUGINID(VEND_CORE, 0x0F),
    ID_CLUMP = MAKEPLUGINID(VEND_CORE, 0x10),
    ID_LIGHT = MAKEPLUGINID(VEND_CORE, 0x12),
    ID_ATOMIC = MAKEPLUGINID(VEND_CORE, 0x14),
    ID_TEXTURENATIVE = MAKEPLUGINID(VEND_CORE, 0x15),
    ID_TEXDICTIONARY = MAKEPLUGINID(VEND_CORE, 0x16),
    ID_IMAGE = MAKEPLUGINID(VEND_CORE, 0x18),
    ID_GEOMETRYLIST = MAKEPLUGINID(VEND_CORE, 0x1A),
    ID_ANIMANIMATION = MAKEPLUGINID(VEND_CORE, 0x1B),
    ID_RIGHTTORENDER = MAKEPLUGINID(VEND_CORE, 0x1F),
    ID_UVANIMDICT = MAKEPLUGINID(VEND_CORE, 0x2B),

    // Toolkit
    ID_SKYMIPMAP = MAKEPLUGINID(VEND_CRITERIONTK, 0x10),
    ID_SKIN = MAKEPLUGINID(VEND_CRITERIONTK, 0x16),
    ID_HANIM = MAKEPLUGINID(VEND_CRITERIONTK, 0x1E),
    ID_USERDATA = MAKEPLUGINID(VEND_CRITERIONTK, 0x1F),
    ID_MATFX = MAKEPLUGINID(VEND_CRITERIONTK, 0x20),
    ID_ANISOT = MAKEPLUGINID(VEND_CRITERIONTK, 0x27),
    ID_PDS = MAKEPLUGINID(VEND_CRITERIONTK, 0x31),
    ID_ADC = MAKEPLUGINID(VEND_CRITERIONTK, 0x34),
    ID_UVANIMATION = MAKEPLUGINID(VEND_CRITERIONTK, 0x35),

    // World
    ID_MESH = MAKEPLUGINID(VEND_CRITERIONWORLD, 0x0E),
    ID_NATIVEDATA = MAKEPLUGINID(VEND_CRITERIONWORLD, 0x10),
    ID_VERTEXFMT = MAKEPLUGINID(VEND_CRITERIONWORLD, 0x11),

    // anything driver/device related (only as allocation tag)
    ID_DRIVER = MAKEPLUGINID(VEND_DRIVER, 0),

    // RwAnalyze RW_Secs.ini
    //Miscellaneous:
    //# R* Custom Defs // mazahaka ------------
    RWS_ID_Unknown_1 = 0x0253F2F0,
    RWS_ID_Unknown_2 = 0x0253F2F1,
    RWS_ID_Unknown_3 = 0x0253F2F2,
    RWS_ID_Unknown_4 = 0x0253F2F3,
    RWS_ID_Unknown_5 = 0x0253F2F4,
    RWS_ID_Unknown_6 = 0x0253F2F5,
    RWS_ID_Specular_Material = 0x0253F2F6,
    RWS_ID_Unknown_8 = 0x0253F2F7,
    RWS_ID_2dfx = 0x0253F2F8,
    RWS_ID_Night_Vertex_Colors = 0x0253F2F9,
    RWS_ID_Collision_Model = 0x0253F2FA,
    RWS_ID_Unknown_12 = 0x0253F2FB,
    RWS_ID_Reflection_Material = 0x0253F2FC,
    RWS_ID_Mesh_Extension = 0x0253F2FD,
    RWS_ID_Frame = 0x0253F2FE,
    RWS_ID_Unknown_16 = 0x0253F2FF,
};
enum Flags
{
    TRISTRIP = 0x01,
    POSITIONS = 0x02,
    TEXTURED = 0x04,
    PRELIT = 0x08,
    NORMALS = 0x10,
    LIGHT = 0x20,
    MODULATE = 0x40,
    TEXTURED2 = 0x80,
    // When this flag is set the geometry has
    // native geometry. When streamed out this geometry
    // is written out instead of the platform independent data.
    // When streamed in with this flag, the geometry is mostly empty.
    NATIVE = 0x01000000,
    // Just for documentation: RW sets this flag
    // to prevent rendering when executing a pipeline,
    // so only instancing will occur.
    // librw's pipelines are different so it's unused here.
    NATIVEINSTANCE = 0x02000000
};

struct RGBA
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};
struct ChunkHeaderInfo // rwbase.h bool findChunk()
{
    uint32_t type;
    uint32_t length;
    uint32_t version, build;
};
struct GeoStreamData
{
    uint32_t flags;
    int32_t numTriangles;
    int32_t numVertices;
    int32_t numMorphTargets;
};
struct SurfaceProperties
{
    float ambient;
    float specular;
    float diffuse;
};
struct MatStreamData
{
    int32_t flags;	// unused according to RW
    RGBA  color;
    int32_t unused;
    int32_t textured;
};
struct V3d { float x, y, z; };
struct FrameStreamData
{
    V3d right, up, at, pos;
    int32_t parent;
    int32_t matflag;
};


//---my chunk необ€зательно мне так удобнее просто
enum MAZAHAKA_RW_PLUGINS
{
    mzUNDEF = 0x0,
    mzFRAME = 0x1, // hier names
    mzHANIM = 0x2,
    mzBMESH = 0x3,
    mzESKIN = 0x4,
};
struct MAZAHAKA_RW_PLUGIN
{
    uint32_t MAZAHAKA_RW_PLUGIN_ID;
    void* PLUGIN_PAYLOAD;
    uint32_t PLUGIN_SIZE; // b
    static void* Alloc(int sz) { return sz > 0 ? memset(malloc(sz), 0, sz) : NULL; }
    //static void* Alloc(int sz) { void* p = malloc(sz); memset(p, 0, sz); return p; }
};

bool CheckFrameStarts(const std::vector<std::string>& frameNames, std::string frame_name)
{
    bool tolowercheck = true;
    if (tolowercheck)
    {
        for (const auto& prefix : frameNames) {
            if (TOOLS::ToLower(frame_name).find(TOOLS::ToLower(prefix)) == 0) { return true; }
        }
    }
    else
    {
        for (const auto& prefix : frameNames) {
            if (frame_name.find(prefix) == 0) { return true; }
        }
    }
    return false;
}


//#define CHECK_EXTRA_NODES // todo
const std::vector<std::string> mazahaka_vehicleFrames = 
{
        "wheel_rf_dummy", "wheel_rm_dummy", "wheel_rb_dummy", "wheel_lf_dummy", "wheel_lm_dummy", "wheel_lb_dummy",
        "bump_front_dummy", "bonnet_dummy", "wing_rf_dummy", "wing_rr_dummy", "wing_lf_dummy", "wing_lr_dummy",
        "door_rf_dummy", "door_rr_dummy", "door_lf_dummy", "door_lr_dummy", "boot_dummy", "bump_rear_dummy",
        "windscreen_dummy", "ped_frontseat", "ped_backseat", "headlights", "taillights", "exhaust",
        "extra1", "extra2", "extra3", "extra4", "extra5", "extra6", "boat_moving_hi", "boat_rudder_hi",
        "boat_flap_left", "boat_flap_right", "boat_rearflap_left", "boat_rearflap_right", "windscreen",
        "door_lhs_dummy", "door_rhs_dummy", "light_front", "light_rear", "ped_left_entry", "ped_mid_entry",
        "ped_right_entry", "door_front_dummy", "door_back_dummy", "ramp_front_dummy", "ramp_back_dummy",
        "chim_left", "ped_point", "car1_dummy", "car2_dummy", "car3_dummy", "car4_dummy", "chassis_dummy",
        "toprotor", "backrotor", "tail", "topknot", "skid_left", "skid_right", "wheel_front_dummy",
        "wheel_rear_dummy", "light_tailplane", "light_left", "light_right", "forks_front", "forks_rear",
        "wheel_front", "wheel_rear", "mudguard", "handlebars", "rearrotor", "_dam", "_ok", "_hi", "extra",
        "_lo", "_vlo", "boat_hi",
};

const std::vector<std::string> mazahaka_lcs_pedFrames =
{
    "Root", "Pelvis", "Spine", "Spine1", "L Thigh", "R Thigh", "R Calf", "R Foot", "R Toe0", "L Calf",
    "L Foot", "L Toe0", "Neck", "Head", "Bip01 L Clavicle", "Bip01 R Clavicle", "R UpperArm", "R Forearm",
    "R Hand", "R Finger", "L UpperArm", "L Forearm", "L Hand", "L Finger",
};

const std::vector<std::string> mazahaka_weaponsFrames = // check  size 2?
{
    "object", "object_L0",
};


// re3 reVC Vehicle/PedModelInfo.cpp

const std::vector<std::string> mazahaka_re3_pedFrames =
{
    "Smid", "Shead", "Supperarml", "Supperarmr", "SLhand", "SRhand", "Supperlegl", "Supperlegr", "Sfootl", "Sfootr", "Slowerlegr", "Storso",
};

const std::vector<std::string> mazahaka_re3_carIds =
{
    "wheel_rf_dummy",	
    "wheel_rm_dummy",	
    "wheel_rb_dummy",	
    "wheel_lf_dummy",	
    "wheel_lm_dummy",	
    "wheel_lb_dummy",	
    "bump_front_dummy"
    "bonnet_dummy",	
    "wing_rf_dummy",	
    "wing_rr_dummy",	
    "door_rf_dummy",	
    "door_rr_dummy",	
    "wing_lf_dummy",	
    "wing_lr_dummy",	
    "door_lf_dummy",	
    "door_lr_dummy",	
    "boot_dummy",		
    "bump_rear_dummy",
    "windscreen_dummy",
    "ped_frontseat",
    "ped_backseat",
    "headlights",
    "taillights",
    "exhaust",
    "extra1",
    "extra2",
    "extra3",
    "extra4",
    "extra5",
    "extra6",
};

const std::vector<std::string> mazahaka_re3_boatIds = 
{
    "boat_moving_hi",
    "boat_rudder_hi",
    "boat_flap_left",
    "boat_flap_right",
    "boat_rearflap_left",
    "boat_rearflap_right",
    "windscreen",
    "windscreen_hi_ok",
    "ped_frontseat",
};

const std::vector<std::string> mazahaka_re3_trainIds =
{
    "door_lhs_dummy",
    "door_rhs_dummy",
    "light_front",
    "light_rear",
    "ped_left_entry",
    "ped_mid_entry",
    "ped_right_entry",
};

const std::vector<std::string> mazahaka_re3_ferryIds =
{
    "door_front_dummy",
    "door_back_dummy",
    "ramp_front_dummy",
    "ramp_back_dummy",
    "light_front",
    "light_rear",
    "chim_left",
    "ped_point",
    "car1_dummy",
    "car2_dummy",
    "car3_dummy",
    "car4_dummy",
};

const std::vector<std::string> mazahaka_re3_heliIds =
{
   "chassis_dummy",
   "toprotor",
   "backrotor",
   "tail",
   "topknot",
   "skid_left",
   "skid_right",
};

const std::vector<std::string> mazahaka_re3_planeIds =
{
    "wheel_front_dummy",
    "wheel_rear_dummy",
    "light_tailplane",
    "light_left",
    "light_right",
};

const std::vector<std::string> mazahaka_re3_bikeIds =
{
    "chassis_dummy",
    "forks_front",
    "forks_rear",
    "wheel_front",
    "wheel_rear",
    "mudguard",
    "handlebars",
    "ped_frontseat",
    "ped_backseat",
    "headlights",
    "taillights",
    "exhaust",	
    "extra1",
    "extra2",
    "extra3",
    "extra4",
    "extra5",
    "extra6",
};
// todo gta sa, gta vcs frame list hier plg name check!!




//--rwbase.h
inline int
libraryIDUnpackVersion(uint32_t libid)
{
    if (libid & 0xFFFF0000)
        return ((libid >> 14 & 0x3FF00) + 0x30000) |
        (libid >> 16 & 0x3F);
    else
        return libid << 8;
}

inline int
libraryIDUnpackBuild(uint32_t libid)
{
    if (libid & 0xFFFF0000)
        return libid & 0xFFFF;
    else
        return 0;
}

// base.cpp
bool
readChunkHeaderInfo(std::ifstream* file, ChunkHeaderInfo* header)
{
    struct {
        int32_t type, size;
        uint32_t id;
    } buf;
    if (!file) { return false; }
    //s->read32(&buf, 12);
    file->read((char*)&buf, sizeof(buf)); // 12

    //if (s->eof()) { return false; }
    if (!file->good()) { return false; }

    if (!header) { return false; }
    header->type = buf.type;
    header->length = buf.size;
    header->version = libraryIDUnpackVersion(buf.id);
    header->build = libraryIDUnpackBuild(buf.id);
    return true;
}


bool
RwStreamFindChunk(std::ifstream* file, uint32_t type, uint32_t* length, uint32_t* version)
{
    ChunkHeaderInfo header;
    if (!file) { return false; }
    while (readChunkHeaderInfo(file, &header)) {
        if (header.type == rwID_NAOBJECT)
            return false;
        if (header.type == type) {
            if (length)
                *length = header.length;
            if (version)
                *version = header.version;
            return true;
        }
       // s->seek(header.length);
        file->seekg(header.length, std::ios::cur);
        //if (!file->good()) { return false; }
    }
    return false;
}



//----------------------------------------------------------------------------------------------
// Clump[0]->GeometryList[0]->Geometry[N]->MaterialList[0]->Material[N]->Texture([N]?[0]!)
//#define DBG_INFO
#define PUSH_NAMES // its lazy method collect tex names after parsing dff. so todo return readers data
// i dont found simple method automatize RwAnalyze Texture List Dumper so..
#ifdef PUSH_NAMES
// matlist
struct data
{
    //uint32_t filterAddressing = 0;
    std::string name = "";
    std::string mask = "";
};
std::vector<data> gvreq;
enum DFF_MODELS_TYPES
{
    MI_TYPE_ERROR = -1,
    MI_TYPE_OTHER = 0,
    MI_TYPE_VEHICLE = 1,
    MI_TYPE_PED = 2,
    MI_TYPE_WEAPON = 3,
};
int gitype = DFF_MODELS_TYPES::MI_TYPE_ERROR; // 0 other, 1 vehicle, 2 ped

// out data
//std::vector<std::string> gvtexsreq;
#include <unordered_set>
std::vector<std::string> CollectUniqueTextureNames(const std::vector<data>& materials, bool mask = true)
{
    std::unordered_set<std::string> uniqueTextures;
    /*for (const auto& mat : materials) {
        // ƒобавл€ем им€ текстуры в множество, если не пустое
        if (!mat.name.empty() && uniqueTextures.find(mat.name) == uniqueTextures.end()) {
            uniqueTextures.insert(mat.name);
        }
    }*/

    for (const auto& mat : materials) {
        // ѕроверка на уникальность и добавление имени текстуры, если не пустое
        if (!mat.name.empty()) {
            auto result = uniqueTextures.insert(mat.name);
            //if (result.second) {  // ≈сли элемент был добавлен (не был дубликатом)
            //}
        }

        // ѕроверка на уникальность и добавление имени маски, если не пустое
        if (mask)
        {
            if (!mat.mask.empty()) {
                auto result = uniqueTextures.insert(mat.mask);
                //if (result.second) {  // ≈сли элемент был добавлен (не был дубликатом)
                //}
            }
        }
    }

    // ѕереносим уникальные имена текстур в вектор дл€ вывода
    return std::vector<std::string>(uniqueTextures.begin(), uniqueTextures.end());
}
#endif

#ifdef DBG_INFO
__declspec(noinline)
#pragma optimize("", off)
#endif
std::vector<MAZAHAKA_RW_PLUGIN> ReadPlugin(std::ifstream* file) // PluginList::streamSkip(Stream *stream) typr
{
    // ped: ext->HAnim PLG, Frame R*
    std::vector<MAZAHAKA_RW_PLUGIN> plugins;// = new std::vector<MAZAHAKA_RW_PLUGIN>;
    int32_t length;
    ChunkHeaderInfo header; // справа инфа в rwanalyze
    if (!file) { return plugins; }
    if (!RwStreamFindChunk(file, rwID_EXTENSION, (uint32_t*)&length, null)) { return plugins; }
    while (length > 0)
    {
        if (!readChunkHeaderInfo(file, &header)) { return plugins; }
        length -= 12; // header
        //std::cout << header.type << "\n"; // each plugin
        //--frame list plugins
        //ID_HANIM
        //RWS_ID_Frame 

        // Geometry pluhin
        // ID_MESH  Bin Mesh OLG
        // ID_SKIN  Skin PLG
        MAZAHAKA_RW_PLUGIN* plg = null;
        int spos = file->tellg(); // tmp
        if (header.type == ID_HANIM) // ID!! not rwID
        {
            plg = new MAZAHAKA_RW_PLUGIN();
            plg->MAZAHAKA_RW_PLUGIN_ID = mzHANIM;
            plg->PLUGIN_SIZE = header.length;
            plg->PLUGIN_PAYLOAD = MAZAHAKA_RW_PLUGIN::Alloc(plg->PLUGIN_SIZE);
            file->read((char*)plg->PLUGIN_PAYLOAD, plg->PLUGIN_SIZE);
        }
        else if (header.type == RWS_ID_Frame)
        {
            plg = new MAZAHAKA_RW_PLUGIN();
            plg->MAZAHAKA_RW_PLUGIN_ID = mzFRAME;
            plg->PLUGIN_SIZE = header.length;
            plg->PLUGIN_PAYLOAD = MAZAHAKA_RW_PLUGIN::Alloc(plg->PLUGIN_SIZE);
            file->read((char*)plg->PLUGIN_PAYLOAD, plg->PLUGIN_SIZE);
        }
        else if (header.type == ID_MESH)
        {
            plg = new MAZAHAKA_RW_PLUGIN();
            plg->MAZAHAKA_RW_PLUGIN_ID = mzBMESH;
            plg->PLUGIN_SIZE = header.length;
            plg->PLUGIN_PAYLOAD = MAZAHAKA_RW_PLUGIN::Alloc(plg->PLUGIN_SIZE);
            file->read((char*)plg->PLUGIN_PAYLOAD, plg->PLUGIN_SIZE);
        }
        else if (header.type == ID_SKIN)
        {
            plg = new MAZAHAKA_RW_PLUGIN();
            plg->MAZAHAKA_RW_PLUGIN_ID = mzESKIN;
            plg->PLUGIN_SIZE = header.length;
            plg->PLUGIN_PAYLOAD = MAZAHAKA_RW_PLUGIN::Alloc(plg->PLUGIN_SIZE);
            file->read((char*)plg->PLUGIN_PAYLOAD, plg->PLUGIN_SIZE);
        }
        file->seekg(spos, std::ios::beg);

        if (plg) { plugins.push_back(*plg); }
        /*FORLIST(lnk, this->plugins){
			Plugin *p = PLG(lnk);
			if(p->id == header.type && p->read){
				p->read(stream, header.length,
				        object, p->offset, p->size);
				goto cont;
			}
		}*/
        file->seekg(header.length, std::ios::cur);
    cont:
        length -= header.length;
    }
    return plugins;
}
#ifdef DBG_INFO
//#pragma optimize("", on)
#endif


#ifdef DBG_INFO
__declspec(noinline)
#endif
void* ReadTexture(std::ifstream* file)
{
    uint32_t length = 0;
    uint32_t filterAddressing = 0;
    char name[128], mask[128];
    if (!file) { return null; }
    if (!RwStreamFindChunk(file, rwID_STRUCT, null, null)) { return null; }
#ifdef DBG_INFO
    std::cout << "FIND! rwID_STRUCT\n";
#endif
    file->read((char*)&filterAddressing, 4);
    if ((filterAddressing & 0xF000) == 0) { filterAddressing |= (filterAddressing & 0xF00) << 4; } // :/

    if (!RwStreamFindChunk(file, rwID_STRING, &length, null)) { return null; }
#ifdef DBG_INFO
    std::cout << "FIND! rwID_STRING\n";
#endif
    file->read(name, length);

    if (!RwStreamFindChunk(file, rwID_STRING, &length, null)) { return null; }
#ifdef DBG_INFO
    std::cout << "FIND! rwID_STRING\n";
#endif
    file->read(mask, length);
    ReadPlugin(file);

//-------------------------------------------------------
#ifdef PUSH_NAMES
    gvreq.push_back({ name , mask });
#endif
//-------------------------------------------------------
}

#ifdef DBG_INFO
__declspec(noinline)
#endif
void* ReadMaterial(std::ifstream* file)
{
    uint32_t length, version = 0;
    MatStreamData buf;
    if (!file) { return null; }
    if (!RwStreamFindChunk(file, rwID_STRUCT, null, &version)) { return null; }
#ifdef DBG_INFO
    std::cout << "FIND! rwID_STRUCT\n";
#endif
    file->read((char*)&buf, sizeof(buf));
    if (!(version < 0x30400)) { file->seekg(sizeof(SurfaceProperties), std::ios::cur); }
    if(buf.textured)
    {
        if (!RwStreamFindChunk(file, rwID_TEXTURE, null, null)) { return null; }
#ifdef DBG_INFO
        std::cout << "FIND! rwID_TEXTURE\n";
#endif
        void* t = ReadTexture(file);
    }
    ReadPlugin(file);
}

#ifdef DBG_INFO
__declspec(noinline)
#endif
void* ReadMaterialList(std::ifstream* file)
{
    int32_t buff = 0;
    int32_t numMat = 0;
    int32_t* indices = null;
    if (!file) { return null; }
    if (!RwStreamFindChunk(file, rwID_STRUCT, null, null)) { return null; }
#ifdef DBG_INFO
    std::cout << "FIND! rwID_STRUCT\n";
#endif
    file->read((char*)&numMat, 4);
    if (numMat == 0) { return null; }
    indices = (int32_t*)malloc(numMat * 4);
    file->read((char*)indices, numMat * 4);

    for(int32_t i = 0; i < numMat; i++)
    {
        if (indices[i] >= 0) {}
        else
        {
            if (!RwStreamFindChunk(file, rwID_MATERIAL, null, null)) { return null; }
#ifdef DBG_INFO
            std::cout << "FIND! rwID_MATERIAL\n";
#endif
            void* m = ReadMaterial(file);
		}
	}
    return null;
}

#ifdef DBG_INFO
__declspec(noinline)
#endif
void* ReadGeometry(std::ifstream* file)
{
    //return null;
    if (!file) { return null; }
    //----uint32_t buf32 = 0;
    uint32_t version = 0;
    if (!RwStreamFindChunk(file, rwID_STRUCT, null, &version)) { return null; }
#ifdef DBG_INFO
    std::cout << "FIND! rwID_STRUCT\n";
#endif
    GeoStreamData buf;
    SurfaceProperties surfProps;
    file->read((char*)&buf, sizeof(buf));

    if (version < 0x34000) { file->read((char*)&surfProps, 12); }
    //Geometry* Geometry::create
    uint32_t geo_flags = buf.flags & 0xFF00FFFF;
    int32_t geo_numTexCoordSets = (geo_flags & 0xFF0000) >> 16;
    int32_t geo_numMorphTargets = 1; // geo->addMorphTargets(1); Geometry*  Geometry::create
    if ((buf.numMorphTargets - 1) != 0) { geo_numMorphTargets += (buf.numMorphTargets - 1); } // Geometry::addMorphTargets(int32 n)
    if (geo_numTexCoordSets == 0) { geo_numTexCoordSets = (geo_flags & TEXTURED) ? 1 : (geo_flags & TEXTURED2) ? 2 : 0; }

    //printf("geo_flags: %d\n", geo_flags);
    //printf("geo_numTexCoordSets: %d\n", geo_numTexCoordSets);
    //printf("geo_numMorphTargets: %d\n", geo_numMorphTargets);

    if (!(geo_flags & NATIVE)) {
        if (geo_flags & PRELIT)
        {
            //file->read((char*)&buf32, 4 * buf.numVertices); 
            file->seekg(4 * buf.numVertices, std::ios::cur);
        }
        for (int32_t i = 0; i < geo_numTexCoordSets; i++)
        {
            //file->read((char*)&buf32, 2 * buf.numVertices * 4);
            file->seekg(2 * buf.numVertices * 4, std::ios::cur);
        }
        for (int32_t i = 0; i < buf.numTriangles; i++)
        {
            //file->read((char*)&buf32, 8);
            file->seekg(8, std::ios::cur);
        }
    }

    for (int32_t i = 0; i < geo_numMorphTargets; i++)
    {
        file->seekg(4*4, std::ios::cur);
        int32_t hasVertices = 0;
        int32_t hasNormals = 0;
        file->read((char*)&hasVertices, 4);
        file->read((char*)&hasNormals, 4);

        if (hasVertices)
        {
            file->seekg(3 * buf.numVertices * 4, std::ios::cur);
        }
        if (hasNormals)
        {
            file->seekg(3 * buf.numVertices * 4, std::ios::cur);
        }
    }



    if (!RwStreamFindChunk(file, rwID_MATLIST, null, null)) { return null; }
#ifdef DBG_INFO
    std::cout << "FIND! rwID_MATLIST\n";
#endif

    // matlist read
    void* ml = ReadMaterialList(file);

    // plugin read offset
    ReadPlugin(file);
    return null;
}

#ifdef DBG_INFO
__declspec(noinline)
#endif
void* ReadFramelist(std::ifstream* file)
{
    int32_t numFrames = 0;
    FrameStreamData buf;
    std::vector<MAZAHAKA_RW_PLUGIN> plglist;
    if (!file) { return null; }
    if (!RwStreamFindChunk(file, rwID_STRUCT, null, null)) { return null; }
#ifdef DBG_INFO
    std::cout << "FIND! rwID_STRUCT\n";
#endif
    file->read((char*)&numFrames, 4);
#ifdef DBG_INFO
    std::cout << "numFrames: " << numFrames << "\n"; // note ped 25 FrameStreamData, 25 extension
#endif
    //for (int32_t i = 0; i < numFrames; i++)
    //{
    //    file->read((char*)&buf, sizeof(buf));
    //}
    file->seekg(numFrames * sizeof(buf), std::ios::cur); // skip struct
    for (int32_t i = 0; i < numFrames; i++)
    {
        std::vector<MAZAHAKA_RW_PLUGIN> lplglist = ReadPlugin(file);
        for (size_t j = 0; j < lplglist.size(); j++) { if (lplglist[j].MAZAHAKA_RW_PLUGIN_ID == mzFRAME) { plglist.push_back(lplglist[j]); } } // cpy + filter
    }

    // dump plugins
    //for (size_t i = 0; i < plglist.size(); i++)
    //{
    //    std::cout << "plgid: " << plglist[i].MAZAHAKA_RW_PLUGIN_ID << "\n";
    //}

    //bool isVeh = false;
    //bool isPed = false;
    for (size_t i = 0; i < plglist.size(); i++)
    {
        //std::cout << "plgid: " << plglist[i].MAZAHAKA_RW_PLUGIN_ID << "\n";
        if (plglist[i].MAZAHAKA_RW_PLUGIN_ID == mzFRAME)
        {
            //isVeh = CheckFrameStarts(vehicleFrames, std::string((char*)plglist[i].PLUGIN_PAYLOAD));
            //isPed = CheckFrameStarts(pedFrames, std::string((char*)plglist[i].PLUGIN_PAYLOAD)); // cnt 25?
            ////isPed = (isPed && (plglist.size() == pedFrames.size())); // дл€ этой проверки нужно удалить дубликаты из plglist
            //gitype = 0; // default other
            //if (isVeh) { gitype = 1; break; }
            //if (isPed) { gitype = 2; break; }

            ////if (isVeh || isPed) { break; }

            gitype = DFF_MODELS_TYPES::MI_TYPE_OTHER; // default
            // vehs
            if (CheckFrameStarts(mazahaka_vehicleFrames, std::string((char*)plglist[i].PLUGIN_PAYLOAD))) { gitype = DFF_MODELS_TYPES::MI_TYPE_VEHICLE; break; }
            else if (CheckFrameStarts(mazahaka_re3_carIds, std::string((char*)plglist[i].PLUGIN_PAYLOAD))) { gitype = DFF_MODELS_TYPES::MI_TYPE_VEHICLE; break; }
            else if (CheckFrameStarts(mazahaka_re3_boatIds, std::string((char*)plglist[i].PLUGIN_PAYLOAD))) { gitype = DFF_MODELS_TYPES::MI_TYPE_VEHICLE; break; }
            else if (CheckFrameStarts(mazahaka_re3_trainIds, std::string((char*)plglist[i].PLUGIN_PAYLOAD))) { gitype = DFF_MODELS_TYPES::MI_TYPE_VEHICLE; break; }
            else if (CheckFrameStarts(mazahaka_re3_ferryIds, std::string((char*)plglist[i].PLUGIN_PAYLOAD))) { gitype = DFF_MODELS_TYPES::MI_TYPE_VEHICLE; break; }
            else if (CheckFrameStarts(mazahaka_re3_heliIds, std::string((char*)plglist[i].PLUGIN_PAYLOAD))) { gitype = DFF_MODELS_TYPES::MI_TYPE_VEHICLE; break; }
            else if (CheckFrameStarts(mazahaka_re3_planeIds, std::string((char*)plglist[i].PLUGIN_PAYLOAD))) { gitype = DFF_MODELS_TYPES::MI_TYPE_VEHICLE; break; }
            else if (CheckFrameStarts(mazahaka_re3_bikeIds, std::string((char*)plglist[i].PLUGIN_PAYLOAD))) { gitype = DFF_MODELS_TYPES::MI_TYPE_VEHICLE; break; }

            // peds
            else if (CheckFrameStarts(mazahaka_lcs_pedFrames, std::string((char*)plglist[i].PLUGIN_PAYLOAD))) { gitype = DFF_MODELS_TYPES::MI_TYPE_PED; break; }
            else if (CheckFrameStarts(mazahaka_re3_pedFrames, std::string((char*)plglist[i].PLUGIN_PAYLOAD))) { gitype = DFF_MODELS_TYPES::MI_TYPE_PED; break; }
        
            // objects (possible weapons, others) ---------------------------------------------------------------------------------\/ dont forget filter list by frames upper
            else if (CheckFrameStarts(mazahaka_weaponsFrames, std::string((char*)plglist[i].PLUGIN_PAYLOAD)) && (plglist.size() == 2)) { gitype = DFF_MODELS_TYPES::MI_TYPE_WEAPON; break; }
        }
    }
    //gitype = isVeh ? 1 : (isPed ? 2 : 0);


    // free
    for (size_t i = 0; i < plglist.size(); i++) { free(plglist[i].PLUGIN_PAYLOAD); }

    //ReadPlugin(file);
    return null;
}

#ifdef DBG_INFO
__declspec(noinline)
#endif
std::vector<std::string> LoadDFF(const char* path, bool add_alpha_in_total_list = true)
{
    std::vector<std::string> res;
    gvreq.clear();
    gitype = DFF_MODELS_TYPES::MI_TYPE_ERROR;
    //rwID_CLUMP->rwID_GEOMETRYLIST->rwID_STRUCT->rwID_GEOMETRY->
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) { std::cout << "Error opening file: " << path << std::endl; return res; }
    if (!RwStreamFindChunk(&file, rwID_CLUMP, null, null)) { return res; }
#ifdef DBG_INFO
    std::cout << "FIND! rwID_CLUMP\n";
#endif
   // if (!RwStreamFindChunk(&file, rwID_FRAMELIST, null, null)) { return res; } // return?
#ifdef DBG_INFO
   // std::cout << "FIND! rwID_FRAMELIST\n";
#endif
    if (RwStreamFindChunk(&file, rwID_FRAMELIST, null, null))
    {
#ifdef DBG_INFO
        std::cout << "FIND! rwID_FRAMELIST\n";
#endif
        void* fl = ReadFramelist(&file);
    }


    if (!RwStreamFindChunk(&file, rwID_GEOMETRYLIST, null, null)) { return res; }
#ifdef DBG_INFO
    std::cout << "FIND! rwID_GEOMETRYLIST\n";
#endif
    if (!RwStreamFindChunk(&file, rwID_STRUCT, null, null)) { return res; }
#ifdef DBG_INFO
    std::cout << "FIND! rwID_STRUCT\n";
#endif
    int32_t numGeometries = 0;
    file.read((char*)&numGeometries, sizeof(numGeometries)); // clump.cpp Clump* Clump::streamRead(Stream * stream)
#ifdef DBG_INFO
    std::cout << "numGeometries: " << numGeometries << "\n";
#endif
    // alloc
    if (numGeometries) {
        //size_t sz = numGeometries * sizeof(Geometry*);
        //void* p = null;
        //p =  malloc(sz);
        //memset(p, 0, sz);
    }
    for (int32_t i = 0; i < numGeometries; i++) {
        if (!RwStreamFindChunk(&file, rwID_GEOMETRY, null, null)) { break; }
#ifdef DBG_INFO
        std::cout << "FIND! rwID_GEOMETRY [" << i+1 << "]\n";
#endif
        void* geo = ReadGeometry(&file);
    }
    //file.read((char*)(&anpk), sizeof(anpk));
    //ROUNDSIZE(anpk.size);
    //file.read((char*)(&dummy), sizeof(dummy)); //42 unk
    //file.seekg(her->numSequences * (100 + 8), std::ios::cur); // 100 anim+kr00 не все 100

#ifdef PUSH_NAMES

/*
#define ALL_

#ifndef ALL
    //http://gta.rockstarvision.com/lcs_files.txt показывает что альфа также в txd мдЄт
    bool add_alpha_in_total_list = true; // false без маски, так совпадает с txd от vehicle
     std::vector<std::string> vtexsreq = CollectUniqueTextureNames(gvreq, add_alpha_in_total_list);
    for (size_t i = 0; i < vtexsreq.size(); i++)
#else
    for (size_t i = 0; i < gvreq.size(); i++)
#endif
    {
#ifndef ALL
        std::cout << i+1 << ": name: " << vtexsreq[i] << "\n"; // tex
#else
        std::cout << i+1 << ": name: " << gvreq[i].name << "   mask: " << gvreq[i].mask << "\n"; // data
#endif
    }
#undef ALL
 */

    //bool add_alpha_in_total_list = true; // false без маски, так совпадает с txd от vehicle
    res = CollectUniqueTextureNames(gvreq, add_alpha_in_total_list);

#endif
    return res;
}

//__declspec(noinline)
void test()
{
    const char* dff = "taxi.dff"; // vehicle 1
    dff = "bar_barrier12.dff"; // other 0
    //dff = "player.dff"; // ped 2 skin plg
    //dff = "vice2.dff"; // ped 2 skin plg
    dff = "m16.dff"; // weapon

    std::vector<std::string> texes = LoadDFF(dff);
    for (size_t i = 0; i < texes.size(); i++)
    {
        std::cout << i + 1 << ": name: " << texes[i] << "\n"; // tex
    }
    std::cout << "type: " << (gitype == MI_TYPE_VEHICLE ? "veh" : (gitype == MI_TYPE_PED ? "ped" : (gitype == MI_TYPE_WEAPON ? "weapon" : "other"))) << "\n"; // todo enum typed?
}


void Dump()
{
    SetCurrDir();
    const char* dffs_dir = "input_dffs";
    const char* out_dir = "log";
    std::vector<std::string> fs = LoadAllFilesFromFolder(dffs_dir, "dff");
    size_t totalFiles = fs.size();
    //std::cout << "cnt: " << fs.size() << "\n";
    //SetCurrDir(dffs_dir); // scandir is no relative
    for (size_t i = 0; i < totalFiles; i++)
    {
        bool add_alpha_in_total_list = true; // mask
        std::vector<std::string> texes = LoadDFF(fs[i].c_str(), add_alpha_in_total_list);
        TOOLS::FileWriteAllLines(std::string(out_dir) + "\\" + TOOLS::GetFileNameFromPath(fs[i], true) + ".txt", texes);

        //for (size_t j = 0; j < texes.size(); j++)
        {
            //std::cout << j + 1 << ": name: " << texes[j] << "\n"; // tex
        }

        // acii
        int progress = static_cast<int>((static_cast<double>(i + 1) / totalFiles) * 100);

        std::cout << "MaZaHaKa Conv: [";
        int progressBarWidth = 50; // ширина полосы
        int pos = progressBarWidth * progress / 100;
        for (int p = 0; p < progressBarWidth; ++p) {
            if (p < pos) std::cout << "=";
            else if (p == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << progress << "%\n";
    }
}

void SortVehPeds()
{
    SetCurrDir();
    const char* dffs_dir = "input_dffs";
    const char* out_dir = "sort";
    std::string vp = std::string(out_dir) + "\\vehs";
    std::string pp = std::string(out_dir) + "\\peds";
    std::string wp = std::string(out_dir) + "\\possible_weap";
    std::string op = std::string(out_dir) + "\\others";

    TOOLS::createDirectory(std::string(out_dir));
    TOOLS::createDirectory(vp);
    TOOLS::createDirectory(pp);
    TOOLS::createDirectory(wp);
    TOOLS::createDirectory(op);

    std::vector<std::string> fs = LoadAllFilesFromFolder(dffs_dir, "dff");
    size_t totalFiles = fs.size();

    for (size_t i = 0; i < totalFiles; i++)
    {
        //system("cls");
        std::cout << "\n" << i << "/" << totalFiles;// << "\n";
        bool add_alpha_in_total_list = true; // mask
        std::vector<std::string> texes = LoadDFF(fs[i].c_str(), add_alpha_in_total_list); // gitype autoreset
        //std::string fname = TOOLS::GetFileNameFromPath(fs[i], true);
        std::string fnameext = TOOLS::GetFileNameFromPath(fs[i], false);

        if (gitype == MI_TYPE_VEHICLE)
        {
            TOOLS::copyFile(fs[i], vp + "\\" + fnameext);
            std::cout << " " << fnameext << " moved as MI_TYPE_VEHICLE";
        }
        else if (gitype == MI_TYPE_PED)
        {
            TOOLS::copyFile(fs[i], pp + "\\" + fnameext);
            std::cout << " " << fnameext << " moved as MI_TYPE_PED";
        }
        else if (gitype == MI_TYPE_WEAPON)
        {
            TOOLS::copyFile(fs[i], wp + "\\" + fnameext);
            std::cout << " " << fnameext << " moved as (object) MI_TYPE_WEAPON";
        }
        else if (gitype == MI_TYPE_OTHER)
        {
            TOOLS::copyFile(fs[i], op + "\\" + fnameext);
            std::cout << " " << fnameext << " moved as (default) MI_TYPE_OTHER";
        }
        else { std::cout << "ERR choose!!" << "\n"; }
    }
}


int main()
{
    InitConsole(); // 1251
    SetCurrDir();

    //func();
	//test();
    //Dump(); // tex list
    SortVehPeds();

	return 0;
}
