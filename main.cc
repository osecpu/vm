#include <iostream>

class OsecpuBinary {
 public:
  enum class Type {
    kUnknown = 0,
    kRevision1 = 1 << 1,
    kRevision2 = 2 << 1,
    kBackend = 0,
    kFrontend = 1,
  };
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
  Type GetBinType() { return binType; }

 private:
  uint8_t binBuf[bufSize];
  int binByteSize;
  Type binType = Type::kUnknown;
  void CheckBinType();
};

OsecpuBinary::Type operator|=(OsecpuBinary::Type &vL, OsecpuBinary::Type vR) {
  return vL = static_cast<OsecpuBinary::Type>(static_cast<int>(vL) |
                                              static_cast<int>(vR));
}

void OsecpuBinary::CheckBinType() {
  binType = OsecpuBinary::Type::kUnknown;
  if (binBuf[0] == 0x05) {
    if (binBuf[1] == 0xE1) {
      binType = OsecpuBinary::Type::kRevision1;
      if (binBuf[2] != 0x00) binType |= OsecpuBinary::Type::kFrontend;
    } else if (binBuf[1] == 0xE2) {
      binType = OsecpuBinary::Type::kRevision2;
      if (binBuf[2] != 0x00) binType |= OsecpuBinary::Type::kFrontend;
    }
  }
}

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
    std::cout << "Type: " << (int)osebin.GetBinType() << std::endl;
    return 0;
  }
  std::cerr << "osecpu: Unknown command: " << argv[1] << std::endl;
  return 1;
}
