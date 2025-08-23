/**
 * @file ImageUtil.h
 * @author ayano
 * @date 1/15/24
 * @brief This is image utils, used to produce images
 */

#ifndef ONEWEEKEND_IMAGEUTIL_H
#define ONEWEEKEND_IMAGEUTIL_H

#include <filesystem>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <tuple>
#include <vector>
#include "IRenderer.h"
#include "KawaiiMQ/kawaiiMQ.h"
#include "MathUtil.h"
#include "spdlog/spdlog.h"
#include "stb_image.h"
using Color = Eigen::Vector3d;

class Image {
public:
	Image();

	~Image();

	Image(const std::string &name, const std::string &parent = IMG_INPUT_DIR);

	bool load(const std::string &name, const std::string &parent = IMG_INPUT_DIR);

	const unsigned char *pixelData(int x, int y) const;

	int width() const;

	int height() const;

private:
	const int bytes_per_pixel = 3;
	int bytes_per_scanline;
	int img_width, img_height;
	unsigned char *data;
};

const std::string grayscale = " .:-=+*#%@";

std::string getGreyScaleCharacter(float r, float g, float b);

std::string makePPM(int width, int height, const std::vector<std::vector<Color>> &img, const std::string &name,
					const std::string &path = IMG_OUTPUT_DIR);

std::string makePPM(const RendererOutput &out, const std::string &name, const std::string &path = IMG_OUTPUT_DIR);

std::string makeGrayscaleTxt(int width, int height, const std::vector<std::vector<Color>> &img, const std::string &name,
							 const std::string &path = IMG_OUTPUT_DIR);

std::string makeGrayscaleString(int width, int height, std::vector<std::vector<Color>> img);

std::string mkdir(const std::string &path, const std::string &name);

std::vector<std::string> split(const std::string &str, const std::string &delimeter);

int clamp(int x, int low, int high);

float gammaCorrect(float c);

struct ImageChunk : public KawaiiMQ::MessageData {
	int startx;
	int starty;
	int chunk_idx;
	int width;
	int height;
	std::vector<std::vector<Color>> partial;
};

#endif // ONEWEEKEND_IMAGEUTIL_H
