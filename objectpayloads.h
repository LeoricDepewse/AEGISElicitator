#ifndef OBJECTPAYLOADS_H
#define OBJECTPAYLOADS_H
#include <string>
#include <vector>
#include "item.h"
#include "integer.h"
#include "dataobject.h"

static const std::vector<std::string> SIGNIFICANCE =
    {"Very Significant", "Significant", "Not very significant", "Minor significance", "Insignificant"};
#ifdef QT_DEBUG
static std::vector<std::string> gAssetType =
    {"Information", "Systems", "Software", "Hardware", "People"};
#else
static std::vector<std::string> gAssetType;
#endif
static bool gAssetType_Sync = false;

#define T_SIGNIFICANCE 0
#define T_ASSETTYPE 1

static const DataProp ASSET[] = {
    {"Name", "theName", text},
    {"Short Code", "theShortCode", text},
    {"Description", "theDescription", largeText},
    {"Significance", "theSignificance", text},
    {"Asset Type", "theType", combo, false, new Integer(T_ASSETTYPE)},
    {"Tags", "theTags", array, true},
    {"", "isCritical", number, true, new Integer(0)},
    {"", "theCriticalRationale", nType, true},
    {"", "theInterfaces", array, true},
    {"", "theEnvironmentProperties", array, true}
};
static const int ASSET_C = (sizeof(ASSET)/sizeof(ASSET[0]));

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

#endif // OBJECTPAYLOADS_H
