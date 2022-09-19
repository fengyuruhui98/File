typedef struct{
    int key;
    UT_hash_handle hh;
}HashItem;

typedef struct Tuple{
    int val[3];
}Tuple;

HashItem * hashFindItem(HashItem ** obj, int key){
    HashItem *pEntry = NULL;
    HASH_FIND_INT(*obj, &key, pEntry);
    return pEntry;
}

bool hashAddItem(HashItem ** obj, int key){
    if(hashFindItem(obj, key)){
        return false;
    }
}