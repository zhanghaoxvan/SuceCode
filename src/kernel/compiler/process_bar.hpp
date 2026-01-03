#pragma once
#include <iostream>
#include <string>
#include <iomanip>

namespace kernel{
    class ProgressBar {
        private:
            int _total_stages;
            int _current_stage;
            std::string _state_names[5];
            int _width;
        
            public:
                ProgressBar(int total_stages = 5, int width = 50) : _total_stages(total_stages), _current_stage(0), _width(width) {
                    _state_names[0] = "Initializing";
                    _state_names[1] = "Compiling";
                    _state_names[2] = "Linking";
                    _state_names[3] = "Running";
                    _state_names[4] = "Finished";
                }

                void setStageName(int stage, std::string &name) {
                    if (stage >= 0 && stage < _total_stages) {
                        _state_names[stage] = name;
                    }

                }

                void nextStage() {
                    if (_current_stage < _total_stages) {
                        _current_stage++;
                        update();
                    }
                }

                void update() {
                    float progress = static_cast<float>(_current_stage) / _total_stages;
                    int pos = static_cast<int>(progress * _width);
                    
                    std::cout << "\r[";
                    for (int i = 0; i < _width; ++i) {
                        if (i < pos)  std::cout << "=";
                        else if (i == pos) std::cout << ">";
                        else std::cout << " ";
                    }
                    std::cout << "] " << std::fixed << std::setprecision(2) << (progress * 100) << "% ";
                    if (_current_stage > 0 && _current_stage <= _total_stages) {
                        std::cout << _state_names[_current_stage-1];
                    }
                    std::cout.flush();

                }

                void finish() {
                    _current_stage = _total_stages;
                    update();
                    std::cout << "    Compilation complete" <<std::endl;

                }

    };
}