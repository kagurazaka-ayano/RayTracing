/**
 * @file ImageUtil.cpp
 * @author ayano
 * @date 1/15/24
 * @brief
 */

#include <algorithm>
#include <cmath>
#include <iterator>
#include <sys/fcntl.h>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "ImageUtil.h"

std::string getGreyScaleCharacter(float r, float g, float b) {
	float sum = r * 255.0 * 0.299 + g * 255.0 * 0.587 + b * 255.0 * 0.114;
	std::stringstream ret;
	ret << grayscale[static_cast<int>((sum - 1.0) / (255.0 / static_cast<float>(grayscale.size())))];
	return ret.str();
}

float gammaCorrect(float c) { return std::pow(c, 1.0 / 2.0); }


std::string makePPM(int width, int height, const std::vector<std::vector<Color>> &img, const std::string &name,
					const std::string &path) {
	auto fout = std::ofstream();
	std::string filepath = mkdir(path, name);
	std::cout << filepath << std::endl;

	fout.open(filepath);
	fout << "P3\n" << width << ' ' << height << "\n255\n";
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			fout << makeColor(img[i][j]);
		}
	}
	fout.close();
	return filepath;
}

std::string makePPM(const RendererOutput &out, const std::string &name, const std::string &path) {
	auto width = out.width, height = out.height;
	std::vector<std::vector<Color>> img;
	for (int i = 0; i < height; i++) {
		std::vector<Color> line;
		for (int j = 0; j < width; j += 3) {
			line.push_back(Color(out.data[i * width], out.data[i * width + 1], out.data[i * width + 2]));
		}
		img.push_back(line);
	}
	return makePPM(width, height, img, name, path);
}

std::string makeGrayscaleString(int width, int height, std::vector<std::vector<Color>> img) {
	std::stringstream ss;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			ss << getGreyScaleCharacter(img[i][j][0], img[i][j][1], img[i][j][2]);
		}
		ss << std::endl;
	}
	return ss.str();
}


std::string makeGrayscaleTxt(int width, int height, const std::vector<std::vector<Color>> &img, const std::string &name,
							 const std::string &path) {
	auto fout = std::ofstream();
	std::string filepath = mkdir(path, name);
	fout.open(filepath);
	fout << makeGrayscaleString(width, height, img);
	fout.close();
	return filepath;
}


std::string mkdir(const std::string &path, const std::string &name) {
	auto dir = path.ends_with("/") ? path : path + "/";
	struct stat st;
	if (stat(dir.c_str(), &st) != 0 && mkdirat(AT_FDCWD, dir.c_str(), 0755) == -1) {
		spdlog::critical("directory create failed at {}", path);
		exit(2);
	}
	return dir + name;
}

std::vector<std::string> split(const std::string &str, const std::string &delimeter) {
	std::vector<std::string> ret;
	std::size_t pos = 0;
	std::string token;
	std::string str_op = str;
	while ((pos = str_op.find(delimeter)) != std::string::npos) {
		token = str_op.substr(0, pos);
		ret.push_back(token);
		str_op.erase(0, pos + delimeter.length());
	}
	return ret;
}

int clamp(int x, int low, int high) {
	if (x > high)
		return high - 1;
	if (x < low)
		return low;
	return x;
}

Image::Image() : data(nullptr) {}

Image::~Image() { STBI_FREE(data); }

Image::Image(const std::string &name, const std::string &parent) {
	if (!load(name, parent)) {
		spdlog::critical("cannot load image from path: {}, aborting", mkdir(parent, name));
		exit(-1);
	}
}

bool Image::load(const std::string &name, const std::string &parent) {

	auto filepath = mkdir(parent, name);
	auto n = bytes_per_pixel;
	std::cout << filepath;
	data = stbi_load(filepath.c_str(), &img_width, &img_height, &n, bytes_per_pixel);
	bytes_per_scanline = img_width * bytes_per_pixel;
	return data != nullptr;
}

int Image::width() const { return img_width; }

int Image::height() const { return img_height; }

const unsigned char *Image::pixelData(int x, int y) const {
	static unsigned char magenta[] = {255, 0, 255};
	if (data == nullptr)
		return magenta;

	x = clamp(x, 0, img_width);
	y = clamp(y, 0, img_height);

	return data + y * bytes_per_scanline + x * bytes_per_pixel;
}
