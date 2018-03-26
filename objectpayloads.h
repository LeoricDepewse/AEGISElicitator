#ifndef OBJECTPAYLOADS_H
#define OBJECTPAYLOADS_H
#include <string>
#include <vector>

static int nextAssetId=0;
struct Asset
{
    int assetId;
    std::string assetName;
    std::string shortCode;
    std::string assetDescription;
    std::string *assetSig;
    std::string *assetType;
    std::string tags;
};

static std::vector<std::string> significance;
static std::vector<std::string> assetType;

#endif // OBJECTPAYLOADS_H
