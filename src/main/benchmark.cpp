#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <chrono>
#include <art.h>
#include <unordered_map>
#include <queue>
#include "collection.h"
#include "string_utils.h"
#include "collection_manager.h"

using namespace std;

int main(int argc, char* argv[]) {
    system("rm -rf /tmp/typesense-data && mkdir -p /tmp/typesense-data");

    std::vector<field> fields_to_index = { field("title", field_types::STRING, false),
                                           field("points", field_types::INT32, false) };

    Store *store = new Store("/tmp/typesense-data");
    CollectionManager & collectionManager = CollectionManager::get_instance();
    collectionManager.init(store, "abcd", "1234");

    Collection *collection = collectionManager.get_collection("hnstories_direct");
    if(collection == nullptr) {
        collection = collectionManager.create_collection("hnstories_direct", fields_to_index, "points").get();
    }

    std::ifstream infile("/Users/kishore/Downloads/hnstories.jsonl");

    std::string json_line;
    auto begin = std::chrono::high_resolution_clock::now();

    while (std::getline(infile, json_line)) {
        collection->add(json_line);
    }

    infile.close();
    std::cout << "FINISHED INDEXING!" << flush << std::endl;

    /*std::vector<std::string> search_fields = {"title"};

    std::vector<string> queries = {"the", "and", "to", "of", "in"};
    auto counter = 0;
    uint64_t results_total = 0; // to prevent optimizations!

    auto begin = std::chrono::high_resolution_clock::now();

    while(counter < 3000) {
        auto i = counter % 5;
        auto results = collection->search(queries[i], search_fields, "", { }, {sort_field("points", "DESC")}, 1, 10, 1, MAX_SCORE, 0).get();
        results_total += results.size();
        counter++;
    }*/

    long long int timeMillis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - begin).count();
    std::cout << "Time taken: " << timeMillis << "ms" << std::endl;
    //std::cout << "Total: " << results_total << std::endl;
    return 0;
}