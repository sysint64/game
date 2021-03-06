#include "platform.hpp"
#include <jpeglib.h>
#include <setjmp.h>
#include <tmx.h>

static Result<AssetData> loadTexture(RegionMemoryBuffer* memory, const char* assetName);

static Result<AssetData> loadJPEGTexture(RegionMemoryBuffer* memory, const char* assetName);

static Result<AssetData> loadPNGTexture(RegionMemoryBuffer* memory, const char* assetName);

static Result<AssetData> loadShader(RegionMemoryBuffer* memory, const char* assetName);

static Result<AssetData> loadLevel(RegionMemoryBuffer* memory, const char* assetName);

Result<AssetData> platformLoadAssetData(
    RegionMemoryBuffer* memory,
    const AssetType assetType,
    const char* assetName
) {
    switch (assetType) {
        case texture:
            return loadTexture(memory, assetName);

        case sfx:
            return resultCreateGeneralError<AssetData>(
                ErrorCode::LOAD_ASSET,
                "Not implemented yet"
            );

        case music:
            return resultCreateGeneralError<AssetData>(
                ErrorCode::LOAD_ASSET,
                "Not implemented yet"
            );

        case shader:
            return loadShader(memory, assetName);

        case font:
            return resultCreateGeneralError<AssetData>(
                ErrorCode::LOAD_ASSET,
                "Not implemented yet"
            );

        case level:
            return loadLevel(memory, assetName);

        default:
            return resultCreateGeneralError<AssetData>(
                ErrorCode::LOAD_ASSET,
                "Unknown asset type"
            );
    }
}

static Result<AssetData> loadTexture(RegionMemoryBuffer* memory, const char* assetName) {
    char *dot = strrchr(assetName, '.');

    if (!dot) {
        return resultCreateGeneralError<AssetData>(
            ErrorCode::LOAD_ASSET,
            "Texture don't have extension"
        );
    }

    if (strcmp(dot, ".jpeg") == 0 || strcmp(dot, ".jpg") == 0) {
        return loadJPEGTexture(memory, assetName);
    }
    else if (strcmp(dot, ".png") == 0) {
        return loadPNGTexture(memory, assetName);
    }
    else {
        return resultCreateGeneralError<AssetData>(
            ErrorCode::LOAD_ASSET,
            "Unknown texture extension: %s", dot
        );
    }
}

struct JpegErrorMgr {
    jpeg_error_mgr pub;

    jmp_buf setJmpBuffer;
};

static char jpegLastErrorMsg[JMSG_LENGTH_MAX] {};

METHODDEF(void) jpegErrorExit (j_common_ptr cinfo) {
    JpegErrorMgr* err = (JpegErrorMgr*) cinfo->err;
    (*(cinfo->err->format_message))(cinfo, jpegLastErrorMsg);
    longjmp(err->setJmpBuffer, 1);
}

static Result<AssetData> loadJPEGTexture(RegionMemoryBuffer* memory, const char* assetName) {
    char path[256] { 0 };
    platformBuildPath(&path[0], "assets", "textures", assetName);

    FILE* infile = fopen(&path[0], "rb");

    if (infile == nullptr) {
        return resultCreateGeneralError<AssetData>(
            ErrorCode::LOAD_ASSET,
            "Can't open asset: %s", &path[0]
        );
    }

    jpeg_decompress_struct cinfo;
    JpegErrorMgr jerr;

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = jpegErrorExit;

    if (setjmp(jerr.setJmpBuffer)) {
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);

        return resultCreateGeneralError<AssetData>(
            ErrorCode::LOAD_ASSET,
            "JPEG decompress error: %s", &jpegLastErrorMsg[0]
        );
    }

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, 0);

    jpeg_start_decompress(&cinfo);

    const u32 width = cinfo.output_width;
    const u32 height = cinfo.output_height;
    const u32 depth = cinfo.output_components;

    const u64 rowSize = width * depth;
    JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, rowSize, 1);

    const u64 textureSize = width * height * depth;
    const u64 size = sizeof(TextureHeader) + textureSize;
    const auto dataResult = regionMemoryBufferAlloc(memory, size);

    if (resultHasError(dataResult)) {
        return switchError<AssetData>(dataResult);
    }

    u8* data = resultGetPayload(dataResult);

    const TextureHeader textureHeader {
        .width = width,
        .height = height,
        .format = TextureFormat::rgb
    };

    memcpy(data, &textureHeader, sizeof(TextureHeader));
    u8* imageData = data + sizeof(TextureHeader);

    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(imageData, buffer[0], rowSize);
        imageData += rowSize;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);

    AssetData assetData = {
        .size = size,
        .data = data
    };

    return resultCreateSuccess(assetData);
}

static Result<AssetData> loadPNGTexture(RegionMemoryBuffer* memory, const char* assetName) {
    return resultCreateGeneralError<AssetData>(
        ErrorCode::LOAD_ASSET,
        "PNG Not implemented yet"
    );
}

static Result<AssetData> loadShader(RegionMemoryBuffer* memory, const char* assetName) {
    char path[256] { 0 };
    platformBuildPath(&path[0], "assets", "shaders", assetName);

    FILE* file = fopen(&path[0], "r");

    if (file == nullptr) {
        return resultCreateGeneralError<AssetData>(
            ErrorCode::LOAD_ASSET,
            "Can't open asset: %s", &path[0]
        );
    }

    fseek(file, 0L, SEEK_END);
    u64 size = ftell(file);
    rewind(file);

    // NOTE(Andrey): +1 for terminate symbol \0
    Result<u8*> dataResult = regionMemoryBufferAlloc(memory, size + 1);

    if (resultHasError(dataResult)) {
        return switchError<AssetData>(dataResult);
    }

    u8* data = resultGetPayload(dataResult);
    size_t offset = 0;
    char c;

    while ((c = getc(file)) != EOF) {
        data[offset] = c;
        offset += 1;
    }

    data[offset] = 0;
    fclose(file);

    AssetData assetData = {
        .size = size,
        .data = data
    };

    return resultCreateSuccess(assetData);
}

static Result<AssetData> loadLevel(RegionMemoryBuffer* memory, const char* assetName) {
    return resultCreateGeneralError<AssetData>(
        ErrorCode::LOAD_ASSET,
        "Level loading error"
    );
    // tmx_map *map = tmx_load("path/map.tmx");

    // if (!map) {
    //     tmx_perror("tmx_load");

    //     return resultCreateGeneralError<AssetData>(
    //         ErrorCode::LOAD_ASSET,
    //         "Level loading error"
    //     );
    // }

    // tmx_map_free(map);

    // return resultCreateGeneralError<AssetData>(
    //     ErrorCode::LOAD_ASSET,
    //     "Level loading Not implemented yet"
    // );
}
