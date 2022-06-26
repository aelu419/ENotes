#include <fstream>

int main(int argc, char *argv[]) {
    std::ofstream write;
    write.open(argv[2]);
    write << "#ifndef GENERATED_CONSTANT\n#define GENERATED_CONSTANT " << argv[1] << "\n#endif";
    write.close();
}