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

#ifndef CELLDEVS_TUTORIAL_1_1_SPATIAL_SIR_COUPLED_HPP
#define CELLDEVS_TUTORIAL_1_1_SPATIAL_SIR_COUPLED_HPP

#include <nlohmann/json.hpp>
#include <cadmium/celldevs/coupled/grid_coupled.hpp>
#include "state.hpp"
#include "vicinity.hpp"
#include "cells/sir_cell.hpp"

/**
 * We need to define a grid_coupled class that knows all the different types of cells that the scenario may have.
 * @tparam T type used to represent simulation time.
 */
template <typename T>
class sirds_coupled : public cadmium::celldevs::grid_coupled<T, sir, mc> {
public:

    explicit sirds_coupled(std::string const &id) : grid_coupled<T, sir, mc>(id){}

    /**
     * We only have to override the add_grid_cell_json method.
     * We have to match a string containing a cell type with the cell class that corresponds to this type.
     * @param cell_type string that tells us which cell type needs to be loaded
     * @param map information about the scenario (i.e., shape of the scenario, neighbors, vicinity with neighbors...)
     * @param delay_id string that tells us which delay type (transport, hybrid, or inertial) must implement the cell
     * @param config chunk of JSON file with additional configuration parameters. We won't use this yet.
     */
    void add_grid_cell_json(std::string const &cell_type, cell_map<sir, mc> &map, std::string const &delay_id,
                            nlohmann::json const &config) override {
        if (cell_type == "sir") {
            // In this first example, we only have one cell type: the sir cell.
            // We only have to call the add_cell method with the corresponding cell type in the template
            this->template add_cell<sir_cell>(map, delay_id);
        } else throw std::bad_typeid();
    }
};

#endif //CELLDEVS_TUTORIAL_1_1_SPATIAL_SIR_COUPLED_HPP
