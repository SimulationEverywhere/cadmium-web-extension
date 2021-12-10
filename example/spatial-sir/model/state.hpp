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

#ifndef CELLDEVS_TUTORIAL_1_1_SPATIAL_SIR_STATE_HPP
#define CELLDEVS_TUTORIAL_1_1_SPATIAL_SIR_STATE_HPP

#include <nlohmann/json.hpp>

/**
 * IN OUR EXAMPLE, CELLS' STATE WILL BE REPRESENTED WITH AN OBJECT OF THE SIR STRUCT
 */
struct sir {
    unsigned int population;    /// Number of individuals that live in the cell
    float susceptible;          /// Percentage (from 0 to 1) of people that are susceptible to the disease
    float infected;             /// Percentage (from 0 to 1) of people that are infected
    float recovered;            /// Percentage (from 0 to 1) of people that already recovered from the disease
    sir() : population(0), susceptible(1), infected(0), recovered(0) {}  // a default constructor is required
    sir(unsigned int pop, float s, float i, float r) : population(pop), susceptible(s), infected(i), recovered(r) {}
};

/**
 * We need to implement the != operator for the desired cell state struct.
 * Otherwise, Cadmium will not be able to detect a state change and work properly
 * @param x first state struct to compare
 * @param y second state struct to compare
 * @return true if x and y contain different data
 */
inline bool operator != (const sir &x, const sir &y) {
    return x.population != y.population ||
           x.susceptible != y.susceptible || x.infected != y.infected || x.recovered != y.recovered;
}

/**
 * We need to implement the << operator for the desired cell state struct.
 * Otherwise, Cadmium will not be able to print the cell state in the output log file
 * @param os output stream (usually, the log file)
 * @param x state struct to print
 * @return the output stream with the cell state already printed
 */
std::ostream &operator << (std::ostream &os, const sir &x) {
    os << x.population << "," << x.susceptible << "," << x.infected << "," << x.recovered;
    return os;
}

/**
 * We need to implement the from_json method for the desired cell state struct.
 * Otherwise, Cadmium will not be able to understand the JSON configuration file.
 * @param j Chunk of JSON file that represents a cell state
 * @param s cell state struct to be filled with the configuration shown in the JSON file.
 */
[[maybe_unused]] void from_json(const nlohmann::json& j, sir &s) {
    j.at("population").get_to(s.population);
    j.at("susceptible").get_to(s.susceptible);
    j.at("infected").get_to(s.infected);
    j.at("recovered").get_to(s.recovered);
}

#endif //CELLDEVS_TUTORIAL_1_1_SPATIAL_SIR_STATE_HPP
