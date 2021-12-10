/**
 * Copyright (c) 2020, Román Cárdenas Rodríguez
 * ARSLab - Carleton University
 * GreenLSI - Polytechnic University of Madrid
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

#ifndef CELLDEVS_TUTORIAL_1_1_SPATIAL_SIR_VICINITY_HPP
#define CELLDEVS_TUTORIAL_1_1_SPATIAL_SIR_VICINITY_HPP

#include <nlohmann/json.hpp>

/**
 * IN OUR EXAMPLE, VICINITY BETWEEN CELLS WILL BE REPRESENTED WITH AN OBJECT OF THE MC STRUCT
 */
struct mc {
    float connectivity;     /// Connectivity factor from 0 to 1 (i.e. how easy it is to move from one cell to another)
    float mobility;         /// Mobility factor from 0 to 1 (i.e. percentage of people that go from one cell to another)
    mc() : connectivity(0), mobility(0) {}  // a default constructor is required
    mc(float c, float m) : connectivity(c), mobility(m) {}
};

/**
 * We need to implement the from_json method for the desired cells vicinity struct.
 * Otherwise, Cadmium will not be able to understand the JSON configuration file.
 * @param j Chunk of JSON file that represents a cell state
 * @param v cells vicinity struct to be filled with the configuration shown in the JSON file.
 */
[[maybe_unused]] void from_json(const nlohmann::json& j, mc &v) {
    j.at("connectivity").get_to(v.connectivity);
    j.at("mobility").get_to(v.mobility);
}

#endif //CELLDEVS_TUTORIAL_1_1_SPATIAL_SIR_VICINITY_HPP
