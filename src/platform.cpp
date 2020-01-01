#include "platform.hpp"
#include "jpeglib.h"
#include <setjmp.h>

static Result<AssetData> loadTexture(const char* assetName);

static Result<AssetData> loadJPEGTexture(const char* assetName);

static Result<AssetData> loadPNGTexture(const char* assetName);

static Result<AssetData> loadShader(const char* assetName);

Result<AssetData> platformLoadAssetData(const AssetType assetType, const char* assetName) {
    switch (assetType) {
        case texture:
            return loadTexture(assetName);

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
            return loadShader(assetName);

        case font:
            return resultCreateGeneralError<AssetData>(
                ErrorCode::LOAD_ASSET,
                "Not implemented yet"
            );

        default:
            return resultCreateGeneralError<AssetData>(
                ErrorCode::LOAD_ASSET,
                "Unknown asset type"
            );
    }
}

static Result<AssetData> loadTexture(const char* assetName) {
    char *dot = strrchr(assetName, '.');

    if (!dot) {
        return resultCreateGeneralError<AssetData>(
            ErrorCode::LOAD_ASSET,
            "Texture don't have extension"
        );
    }

    if (strcmp(dot, ".jpeg") == 0 || strcmp(dot, ".jpg") == 0) {
        return loadJPEGTexture(assetName);
    }
    else if (strcmp(dot, ".png") == 0) {
        return loadPNGTexture(assetName);
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

// TODO(Andrey): Memory managment
static Result<AssetData> loadJPEGTexture(const char* assetName) {
    AssetData assetData;

    const TextureHeader textureHeader {
        .width = 0,
        .height = 0,
        .format = TextureFormat::rgb
    };

    assetData.size += sizeof(TextureHeader);

    jpeg_decompress_struct cinfo;
    JSAMPARRAY buffer;
    int rowStride;
    JpegErrorMgr jerr;

    char path[256] { 0 };
    platformBuildPath(&path[0], "assets", "textures", assetName);

    FILE* infile = fopen(&path[0], "rb");

    if (infile == nullptr) {
        return resultCreateGeneralError<AssetData>(
            ErrorCode::LOAD_ASSET,
            "Can't open asset: %s", &path[0]
        );
    }

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
    jpeg_read_header(&cinfo, true);
    jpeg_start_decompress(&cinfo);
    rowStride = cinfo.output_width * cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, rowStride, 1);

    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        // put_scanline_someplace(buffer[0], rowStride);
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);

    return resultCreateSuccess(assetData);
}

static Result<AssetData> loadPNGTexture(const char* assetName) {
    return resultCreateGeneralError<AssetData>(
        ErrorCode::LOAD_ASSET,
        "PNG Not implemented yet"
    );
}

// TODO(Andrey): Memory managment
static Result<AssetData> loadShader(const char* assetName) {
    char path[256] { 0 };
    char str[10000] { 0 };
    platformBuildPath(&path[0], "assets", "shaders", assetName);

    FILE* infile = fopen(&path[0], "r");

    if (infile == nullptr) {
        return resultCreateGeneralError<AssetData>(
            ErrorCode::LOAD_ASSET,
            "Can't open asset: %s", &path[0]
        );
    }

    while (fgets(str, 10000, infile) != nullptr);

    fclose(infile);
    puts(str);

    AssetData assetData;

    return resultCreateSuccess(assetData);
}
