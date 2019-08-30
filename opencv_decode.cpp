#include <glob.h>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <thread>
#include <tuple>
#include <vector>

// Read file paths into vector
std::vector<std::string> readFilesPaths(const std::string &pattern) {
  std::vector<std::string> files;
  glob_t res;

  glob(pattern.c_str(), GLOB_TILDE, NULL, &res);

  for (size_t i = 0; i < res.gl_pathc; ++i) {
    files.push_back(std::string(res.gl_pathv[i]));
  }

  globfree(&res);

  return files;
}

// Read image data into buffer
std::tuple<std::unique_ptr<char[]>, int>
readFileIntoBuffer(const char *fileName) {
  std::ifstream fileImg(fileName, std::ios::binary);
  fileImg.seekg(0, std::ios::end);
  int bufferLength = fileImg.tellg();
  fileImg.seekg(0, std::ios::beg);

  if (fileImg.fail()) {
    return std::make_tuple(nullptr, 0);
  }

  std::unique_ptr<char[]> buffer(new char[bufferLength]);
  fileImg.read(buffer.get(), bufferLength);

  return std::make_tuple(std::move(buffer), bufferLength);
}

size_t generateNumberInRange(size_t min, size_t max) {
  std::mt19937 rng;

  rng.seed(std::random_device()());
  std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);

  return dist(rng);
}

// Read image file into a buffer and decode
void processFile(int tid, std::vector<std::string> &filesToProcess) {
  int bufferLength = 0, fileIdx = 0;
  char *buffer = nullptr;
  double duration = 0;

  auto start = std::chrono::steady_clock::now();

  for (size_t i = 0; i < filesToProcess.size(); ++i) {
    fileIdx = generateNumberInRange(0, filesToProcess.size() - 1);

    auto bufLenTuple = readFileIntoBuffer(filesToProcess[fileIdx].c_str());

    bufferLength = std::get<1>(bufLenTuple);
    if (bufferLength == 0) {
      printf("Failed to read image %s\n", filesToProcess[fileIdx].c_str());
      return;
    }

    buffer = std::get<0>(bufLenTuple).get();

    // Decode image data
    cv::Mat img;
    img = cv::imdecode(cv::Mat(1, bufferLength, CV_8UC1, buffer),
                       CV_LOAD_IMAGE_UNCHANGED);

    if (i % 1000 == 0) {
      auto end = std::chrono::steady_clock::now();

      duration =
          std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

      if (duration > 0) {
        printf("%d %.0lf%%, %.0lf imgs/s\n", tid,
               100.0 * (double)i / (double)filesToProcess.size(),
               ((double)(i + 1)) / duration);
      }
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Bad numbers of arguments\n");
    printf("opencv_decode.exe 18 \"/data/dataset/imagenet/val/*\"\n");
    return 1;
  }

  int numThreads = atoi(argv[1]);
  std::string pattern(argv[2]);

  std::thread t[numThreads];

  std::vector<std::string> filesToProcess = readFilesPaths(pattern);

  printf("Number of files to process: %zd\n", filesToProcess.size());
  printf("Number of threads: %d\n", numThreads);
  printf("File pattern: %s\n", pattern.c_str());

  for (int i = 0; i < numThreads; ++i) {
    t[i] = std::thread(processFile, i, std::ref(filesToProcess));
  }

  for (int i = 0; i < numThreads; ++i) {
    t[i].join();
  }

  return 0;
}
