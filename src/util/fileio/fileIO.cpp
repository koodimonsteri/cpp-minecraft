#include "util/fileio/fileIO.hpp"

GameConfig FileIO::loadGameConfig(const std::string& path)
{
    std::ifstream ifs;
    ifs.open(path, std::fstream::in);

    GameConfig newConfig;

    if(!ifs.good()) {
        LOG_CRITICAL("---Failed to open file---", path);
    } else {
        char line[MAXLINE];
        std::string line2;
        while(!ifs.eof())
        {
            ifs.getline(line, MAXLINE);
            LOG_WARN(line);
        }
        ifs.close();
        
        LOG_TRACE("Succesfully loaded gameConfig from file. Path=", path);
    }
    return newConfig;
}

void FileIO::writeGameConfig(const GameConfig& gc, const std::string& path)
{
    std::ofstream ofs;
    ofs.open(path, std::fstream::out);

    if(!ofs.good()) {
        LOG_CRITICAL("---Failed to open file---", path);
    } else {
        ofs << "windowWidth=" << gc.windowWidth << "\n";     // Write window size
        ofs << "windowHeight=" << gc.windowHeight << "\n";
        
        if (gc.vSync) { ofs << "vSync=1\n"; }                // Write vSync
        else { ofs << "vSync=0\n"; }
        if(gc.isFullScreen) { ofs << "fullScreen=1\n"; }     // Write fullScreen
        else { ofs << "fullScreen=0\n"; }
        
        ofs << "UPS=" << gc.ups << "\n";                     // Write updates per second

        ofs.close();

        LOG_TRACE("Succesfully wrote gameconfig to file. Path=", path);
    }
}