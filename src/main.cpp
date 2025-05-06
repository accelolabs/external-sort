#include "Helpers.h"
#include <iostream>


int main(int, char**){
        // Write vector to file
        std::vector<int> original = {42, 17, 93, 5, 64};
        vector_to_text(original, "numbers.txt");
        
        // Read back from file
        std::vector<int> restored = text_to_vector("numbers.txt");
        
        // Verify contents
        for (int n : restored) {
            std::cout << n << "\n";
        }
}
