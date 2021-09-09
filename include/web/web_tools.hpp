/**
 * Copyright (c) 2017, Bruno St-Aubin
 * Carleton University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WEB_TOOLS_HPP
#define WEB_TOOLS_HPP

#include <string>
#include <vector>
#include <random>
#include <fstream>

#include <boost/algorithm/string/join.hpp>

#include <external/json.hpp>

using json = nlohmann::json;

namespace cadmium {
    namespace web {
    	class tools {
    	public:
        	static string clean_name(const char* raw_name) {
        		string name = boost::core::demangle(raw_name);

        		return name.substr(name.find_last_of(':') + 1);
        	}

        	static int Random(int min, int max) {
        		if (min == max) return min;

        		return std::rand() % (max - min) + min;
        	}

        	static string trim(string s){
    			// holy crap this is ugly.
        		s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());

        		return s;
        	}

        	static vector<string> split(const string& s, char delimiter) {
        		vector<string> splits;
    			string split;
    			std::istringstream ss(s);

    			while (std::getline(ss, split, delimiter)) splits.push_back(split);

    			return splits;
        	}

        	static json read_geojson(string path) {
        		std::ifstream stream(path);

        		if (!stream.is_open() || stream.fail()) throw strerror(errno);

        		json token = json::parse(stream);

        		stream.close();

        		return token;
        	}

        	static int round_to_int(float x) {
        		// 1.2 >> 1.7 >> 1
        		// 1.6 >> 2.1 >> 2
        		// -1.2 >> -0.7 >> -1.7 >> -1
        		// -1.6 >> -1.1 >> -2.1 >> -2
        		return int(x + 0.5 - (x < 0));
        	}
    	};
    }
}



#endif // WEB_TOOLS_HPP
