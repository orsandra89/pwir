#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "BPM.cpp"
#include "Line.cpp"


std::vector<std::shared_ptr<Drawable>> loadDrawableLinesFromFile(const std::string &filename) {
    std::vector<std::shared_ptr<Drawable>> drawables;
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return drawables;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        int x0, y0, x1, y1;
        if (ss >> x0 >> y0 >> x1 >> y1) {
            drawables.push_back(std::make_shared<Line>(x0, y0, x1, y1, 0, 0, 0));
        }
    }

    return drawables;
}

int main(int argc, char *argv[]) {

    std::map<int, std::string> algorithmMap = {
        {0, "DrawLines"},
        {1, "DrawLinesOpenMP"},
        {2, "DrawLinesThreadLibrary"},
        {3, "DrawElipses"},
        {4, "DrawElipsesOpenMP"},
        {5, "DrawElipsesThreadLibrary"}
    };

    if (argc < 4) {
        std::cout << "Available draw algorithms:" << std::endl;
        for (const auto& pair : algorithmMap) {
            std::cout << pair.first << ". " << pair.second << std::endl;
        }
        std::cerr << "Usage: " << argv[0] << " <algorithm> <input_file> <result_filename>" << std::endl;
        return 1;
    }

    const int width = 100;
    const int height = 100;
    BMP image(width, height);

    std::vector<std::shared_ptr<Drawable>> drawables;
    std::string input_file_path;
    std::string output_file_path;

    int algorithm = std::stoi(argv[1]);

    if (algorithm == 0){
        if (argc == 4){
            input_file_path = argv[2];
            output_file_path = argv[3];

            std::cout << "Load lines from " << input_file_path << ":" << std::endl;
            drawables = loadDrawableLinesFromFile(input_file_path);
        }

        auto start = std::chrono::steady_clock::now();
        
        image.drawAll(drawables);

        auto end = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end - start).count();

        std::cout << "Calculation time: " << elapsed_time << std::endl;

        image.save(output_file_path);

        std::cout << "Image saved to file:  " << output_file_path << std::endl;

    } else {
        std::cerr << "Wrong algorithm selected: " << algorithm << std::endl;
        std::cout << "Available draw algorithms:" << std::endl;
        for (const auto& pair : algorithmMap) {
            std::cout << pair.first << ". " << pair.second << std::endl;
        }
        std::cerr << "Usage: " << argv[0] << " <algorithm> <input_file> <result_filename>" << std::endl;
        return 1;
    }

    return 0;
}
