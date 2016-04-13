#pragma once

#include <stdint.h>
namespace ControlPackets
{
enum {
    eServerVersion=0,
    eOpenSolution,
    eOpenFileAtLine,
    eOpenFiles,
    eSaveAllOpenDocuments,
};
struct StringWithLen
{
    StringWithLen() : len(0) , contents(nullptr) {}
    ~StringWithLen() { delete [] contents; }
    uint16_t len;
    char *contents;
};
struct Packet
{
    int kind;
};
struct ServerVersion : public Packet
{
    uint16_t major;
    uint16_t minor;
};
struct OpenSolution: public Packet
{
    StringWithLen solution_dir;
};
struct OpenFileAtLine : public Packet
{
    StringWithLen filepath;
    uint32_t line_no;
};
struct OpenFiles: public Packet
{
    ~OpenFiles() { delete [] filepaths; }
    uint16_t file_count;
    StringWithLen *filepaths;
};
struct SaveAllOpenDocuments : public Packet
{

};
}
