#include <iostream>

class OsecpuBinary {
 public:
  OsecpuBinary() {}
  const static int bufSize = 1024 * 1024;

  int Load(std::string path) {
    {
      FILE *fp = fopen(path.c_str(), "rb");
      if (!fp) return -1;
      binByteSize = fread(binBuf, 1, bufSize, fp);
      fclose(fp);
    }
    if (bufSize == binByteSize) return -2;
    CheckBinType();
    return binByteSize;
  }
  uint8_t GetBinVersion() { return binVersion; }
  bool GetIsFrontend() { return isFrontend; }

 private:
  uint8_t binBuf[bufSize];
  int binByteSize;
  uint8_t binVersion = 0x00;
  bool isFrontend;
  void CheckBinType() {
    if (binBuf[0] != 0x05) {
      binVersion = 0x00;
      return;
    }
    switch (binBuf[1]) {
      case 0xE1:
      case 0xE2:
        break;
      default:
        binVersion = 0x00;
        return;
    }
    binVersion = binBuf[1];
    isFrontend = binBuf[2] != 0x00;
  }
};

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    std::cout << "Usage: " << std::endl
              << "> osecpu <cmd> [<args> ...]" << std::endl
              << "cmd:" << std::endl
              << "  file <path> : Check osecpu file type" << std::endl;
    return 1;
  }
  if (strcmp(argv[1], "file") == 0) {
    if (argc <= 2) {
      std::cerr << "osecpu: Please specify file path." << std::endl;
      return 1;
    }
    OsecpuBinary osebin;
    osebin.Load(argv[2]);
    std::cout << osebin.GetBinVersion();
    std::cout << "Type: " << osebin.GetBinVersion() << osebin.GetIsFrontend()
              << std::endl;
    return 0;
  }
  std::cerr << "osecpu: Unknown command: " << argv[1] << std::endl;
  return 1;
}
