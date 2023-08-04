#include <stdio.h>
#include "jansson/jansson.h"
#include "base64.h"

typedef struct structData
{
    unsigned int mType;
    char mColName[256];
} structData;

typedef struct structDataList
{
    unsigned int mCount;           /* Number of expressions on the list */
    unsigned int mAlloc;           /* Number of entries allocated below */
    structData mColData[32];

} structDataList;

json_t* serialize_structData(void* aValue) {   
    structData* sValue = (structData*)aValue;
    json_t* sJsonRoot = json_object();
    
    json_object_set_new(sJsonRoot, "mType", json_integer(sValue->mType));
    json_object_set_new(sJsonRoot, "mColName", json_string(sValue->mColName));

    return sJsonRoot;
}

void deserialize_structData(json_t* aJsonRoot, void* aValue) {
    structData* sValue = (structData*)aValue;

    sValue->mType = json_integer_value(json_object_get(aJsonRoot, "mType"));
    strcpy(sValue->mColName, json_string_value(json_object_get(aJsonRoot, "mColName")));
}

void print_structData(void* aValue) {
    structData* sValue = (structData*)aValue;
    printf("structData mType[%d] mColName[%s]\n", sValue->mType, sValue->mColName);
}

json_t* serialize_structDataList(void* aValue) {
    structDataList* sValue = (structDataList*)aValue;
    json_t* sJsonRoot = json_object();

    json_object_set_new(sJsonRoot, "mCount", json_integer(sValue->mCount));
    json_object_set_new(sJsonRoot, "mAlloc", json_integer(sValue->mAlloc));

    json_t* sJsonArray = json_array();
    for (int i = 0; i < sValue->mCount; i++) {
        json_array_append_new(sJsonArray, serialize_structData(&sValue->mColData[i]));
    }
    json_object_set_new(sJsonRoot, "mColData", sJsonArray);

    return sJsonRoot;
}

void deserialize_structDataList(json_t* aJsonRoot, void* aValue) {
    structDataList* sValue = (structDataList*)aValue;

    sValue->mCount = json_integer_value(json_object_get(aJsonRoot, "mCount"));
    sValue->mAlloc = json_integer_value(json_object_get(aJsonRoot, "mAlloc"));

    json_t* sJsonArrayObj = json_object_get(aJsonRoot, "mColData");
    for (int i = 0; i < sValue->mCount; i++) {
        deserialize_structData(json_array_get(sJsonArrayObj, i), &sValue->mColData[i]);
    }
}

void print_structDataList(void* aValue) {
    structDataList* sValue = (structDataList*)aValue;

    printf("structDataList mCount[%d] mAlloc[%d]\n", sValue->mCount, sValue->mAlloc);
    for (int i = 0; i < sValue->mCount; i++) print_structData(&sValue->mColData[i]);
}

json_t* serialize_binary(void* aValue, unsigned int aSize) {
    unsigned char* sValue = (unsigned char*)aValue;
    json_t* sJsonRoot = json_object();

    json_object_set_new(sJsonRoot, "mBinary", json_string(sValue));

    return sJsonRoot;
}

void deserialize_binary(json_t* aJsonRoot, void** aValue, unsigned int aSize) {
    unsigned char* sValue;

    sValue = json_string_value(json_object_get(aJsonRoot, "mBinary"));
    *aValue = sValue;
}

void file_encoding_decoding() {
    unsigned char sBinary[256];
    size_t sBinarySize = 255;
    for (int i = 0; i <= sBinarySize; i++) sBinary[i] = (unsigned char)i+1;
    printf("Binary : [%s]\n", sBinary);


    size_t sOutLen;
    char* sEncode = base64_encode(sBinary, sBinarySize, &sOutLen);

    json_t* sJsonRoot;
    json_error_t sJsonError;

    sJsonRoot = serialize_binary(sEncode, sOutLen);
    char* sDumpStr = json_dumps(sJsonRoot, JSON_ENCODE_ANY);
    printf("Json : [%s]\n", sDumpStr);

    sJsonRoot = json_loads(sDumpStr, JSON_ENCODE_ANY, &sJsonError);
    deserialize_binary(sJsonRoot, &sDumpStr, sOutLen);

    unsigned char* sDecode = base64_decode(sDumpStr, sOutLen, &sOutLen);
    printf("Binary : [%s]\n", sDecode);

    printf("sBuffer - sEncode memcpy:%d\n", memcmp(sBinary, sEncode, sBinarySize));
    printf("sBuffer - sDecode memcpy:%d\n", memcmp(sBinary, sDecode, sBinarySize));
}

int main() {
    printf("Hello, World2!\n");

    json_t* sJsonRoot;
    sJsonRoot = json_object();

    json_t* sJsonObj;
    sJsonObj = json_integer(10);

    json_object_set_new(sJsonRoot, "test", sJsonObj);

    printf("Json Test[%s]\n", json_dumps(sJsonRoot, JSON_ENCODE_ANY));

    test_base64_encoding();
    test_base64_decoding();

    printf("\n\n");

    structDataList sstructDataList;
    sstructDataList.mAlloc = 15;
    sstructDataList.mCount = 3;

    sstructDataList.mColData[0].mType = 10;
    snprintf(sstructDataList.mColData[0].mColName, 256, "col-1");
    sstructDataList.mColData[1].mType = 11;
    snprintf(sstructDataList.mColData[1].mColName, 256, "col-2");
    sstructDataList.mColData[2].mType = 12;
    snprintf(sstructDataList.mColData[2].mColName, 256, "col-3");

    json_t* sJsonRoott = serialize_structDataList(&sstructDataList);
    json_error_t sJsonError;
    char* sDumpStr = json_dumps(sJsonRoott, JSON_ENCODE_ANY);

    printf("[%s\n", sDumpStr);

    sJsonRoot = json_loads(sDumpStr, JSON_ENCODE_ANY, &sJsonError);
    structDataList sstructDataList2;
    deserialize_structDataList(sJsonRoot, &sstructDataList2);

    print_structDataList(&sstructDataList2);

    file_encoding_decoding();

    return 0;
}
