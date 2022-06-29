#include <iostream>
#include <boost/filesystem.hpp>
#include <string>

#pragma region basic
void check_path(const std::string p)
{
    boost::filesystem::path q(p);
    if (boost::filesystem::exists(q))
    {
        std::cout << p << " exists with file size " << boost::filesystem::file_size(q) << "\n";
    }
    else
    {
        std::cout << p << " does not exist\n";
    }
}
void basic(std::string curr)
{
    check_path(curr);
    check_path("fake_path");

    boost::filesystem::path prgm_path(curr);
    if (prgm_path.has_parent_path())
    {
        std::cout << "program is currently in " << prgm_path.parent_path() << std::endl;
    }
    else
    {
        std::cout << "program does not have parent path" << std::endl;
    }
}
#pragma endregion
#pragma region file_status
void file_status(std::string curr)
{
    boost::filesystem::path prgm_path(curr);
    auto stats = boost::filesystem::status(curr);
    auto t = stats.type();

    std::cout << "program is of type " << t << std::endl;
    std::cout << "program has permissions " 
        << (stats.permissions() & boost::filesystem::owner_read ? "r" : "")
        << (stats.permissions() & boost::filesystem::owner_write ? "w" : "")
        << (stats.permissions() & boost::filesystem::owner_exe ? "x" : "")
        << std::endl;
}
#pragma endregion
#pragma region directory_nav
void directory_nav(std::string curr)
{
    boost::filesystem::path prgm_path(curr);
    boost::filesystem::path parent(prgm_path);
    boost::filesystem::directory_iterator end{};

    // iterate everything in the current program's directory
    for(boost::filesystem::directory_iterator i(prgm_path.parent_path()); i != end; i++) {

        boost::filesystem::path p(i->path());
        if (boost::filesystem::is_regular_file(p)) {
            std::cout<<"found file "<<p<<"\n";
        } else if (boost::filesystem::is_directory(p)) {
            std::cout<<"found directory "<<p<<"\n";
        }
    }
}
#pragma endregion

int main(int argc, char *argv[])
{

    /**
     * @brief currently being migrated into STL (std::tr2)
     * files, directories, paths
     * platform-independent
     */

    basic(argv[0]);

    file_status(argv[0]);

    directory_nav(argv[0]);

    return 0;
}