//
// Created by binybrion on 6/30/20.
//

#ifndef PANDEMIC_HOYA_2002_SIR_HPP
#define PANDEMIC_HOYA_2002_SIR_HPP

#include <iostream>
#include <nlohmann/json.hpp>
#include "hysteresis_factor.hpp"

struct sir {
    std::vector<double> age_group_proportions;
    std::vector<double> susceptible;
    std::vector<std::vector<double>> infected;
    std::vector<std::vector<double>> recovered;
    std::vector<double> fatalities;
    std::unordered_map<std::string, hysteresis_factor> hysteresis_factors;
    double population;

    double disobedient;
    double hospital_capacity;
    double fatality_modifier;

    // Required for the JSON library, as types used with it must be default-constructable.
    // The overloaded constructor results in a default constructor having to be manually written.
    sir() = default;

    sir(std::vector<double> susceptible, std::vector<double> infected, std::vector<double> recovered,
        double fatalities, double disobedient, double hospital_capacity, double fatality_modifier) :  // TODO isn't fatalities a vector?
            susceptible{std::move(susceptible)}, infected{std::move(infected)},
            recovered{std::move(recovered)}, fatalities{fatalities}, disobedient{disobedient},
            hospital_capacity{hospital_capacity}, fatality_modifier{fatality_modifier} {}

    unsigned int get_num_age_segments() const {
        return susceptible.size(); // Could use infections.size() or recovered.size(); simply a matter of preference.
    }

    unsigned int get_num_infected_phases() const {
        return infected.front().size(); // There will always be at least one age group, meaning the .front() call is always valid.
    }

    unsigned int get_num_recovered_phases() const {
        return recovered.front().size(); // There will always be at least one age group, meaning the .front() call is always valid.
    }

    static double sum_state_vector(const std::vector<double> &state_vector) {
        return std::accumulate(state_vector.begin(), state_vector.end(), 0.0f);
    }

    // For the get_total_XXX functions, remember that the sum of the values in each vector associated with an age group
    // is one. When looking at the population as a whole, the sum of any state vector has to be adjusted according to how
    // big of a proportion the age group contributes to a population.

    double get_total_fatalities() const {
        double total_fatalities = 0.0f;
        for(int i = 0; i < age_group_proportions.size(); ++i) {
            total_fatalities += fatalities.at(i) * age_group_proportions.at(i);
        }
        return total_fatalities;
    }

    double get_total_infections() const {
        float total_infections = 0.0f;
        for(int i = 0; i < age_group_proportions.size(); ++i) {
            total_infections += sum_state_vector(infected.at(i)) * age_group_proportions.at(i);
        }
        return total_infections;
    }

    double get_total_recovered() const {
        double total_recoveries = 0.0f;
        for(int i = 0; i < age_group_proportions.size(); ++i) {
            total_recoveries += sum_state_vector(recovered.at(i)) * age_group_proportions.at(i);
        }
        return total_recoveries;
    }

    double get_total_susceptible() const {
        double total_susceptible = 0.0f;
        for(int i = 0; i < age_group_proportions.size(); ++i) {
            total_susceptible += susceptible.at(i) * age_group_proportions.at(i);
        }
        return total_susceptible;
    }

    bool operator!=(const sir &other) const {
        return (susceptible != other.susceptible) || (infected != other.infected) || (recovered != other.recovered);
    }
};

bool operator<(const sir &lhs, const sir &rhs) { return true; }

std::ostream &operator<<(std::ostream &os, const sir &sir) {

    // The script included in the Script folder in this project assumes the less detailed output is printed
    // So the following bool should be false. The more detailed output, when the following bool is true, is for
    // the times more information about the various phases is required.
    bool print_specific_state_information = false;

    if(print_specific_state_information) {
        std::string susceptible_information;
        std::string infected_information;
        std::string recovered_information;

        for(auto susceptible_age_segment : sir.susceptible) {
            susceptible_information += "," + std::to_string(susceptible_age_segment);
        }

        for(int i = 0; i < sir.get_num_infected_phases(); ++i) {
            double current_stage_infection = 0.0f;

            for(int j = 0; j < sir.age_group_proportions.size(); ++j) {
                current_stage_infection += sir.infected.at(j).at(i) * sir.age_group_proportions.at(j);
            }

            infected_information += "," + std::to_string(current_stage_infection);
        }

        for(int i = 0; i < sir.get_num_recovered_phases(); ++i) {
            double current_stage_recovered = 0.0f;

            for(int j = 0; j < sir.age_group_proportions.size(); ++j) {
                current_stage_recovered += sir.recovered.at(j).at(i) * sir.age_group_proportions.at(j);
            }

            recovered_information += "," + std::to_string(current_stage_recovered);
        }


        os << sir.population - sir.population * sir.get_total_fatalities() << sir.get_num_age_segments() << ",0" << ","
        << sir.get_num_infected_phases() << "," << sir.get_num_recovered_phases() << ","
            << sir.get_total_susceptible() << infected_information << recovered_information;
    }
    else {


        double new_infections = 0.0f;
        double new_recoveries = 0.0f;

        for(int i = 0; i < sir.age_group_proportions.size(); ++i) {
            new_infections += sir.infected.at(i).at(0) * sir.age_group_proportions.at(i);
            new_recoveries += sir.recovered.at(i).at(0) * sir.age_group_proportions.at(i);
        }

        os << sir.population - sir.population * sir.get_total_fatalities() << "," << sir.get_total_susceptible()
            << "," << sir.get_total_infections() << "," << sir.get_total_recovered() << "," << new_infections << ","
            << new_recoveries << "," << sir.get_total_fatalities() ;
    }

    return os;
}

void from_json(const nlohmann::json &json, sir &current_sir) {
    json.at("age_group_proportions").get_to(current_sir.age_group_proportions);
    json.at("infected").get_to(current_sir.infected);
    json.at("recovered").get_to(current_sir.recovered);
    json.at("susceptible").get_to(current_sir.susceptible);
    json.at("fatalities").get_to(current_sir.fatalities);
    json.at("disobedient").get_to(current_sir.disobedient);
    json.at("hospital_capacity").get_to(current_sir.hospital_capacity);
    json.at("fatality_modifier").get_to(current_sir.fatality_modifier);
    json.at("population").get_to(current_sir.population);
    
    assert(current_sir.age_group_proportions.size() == current_sir.susceptible.size() && current_sir.age_group_proportions.size() == current_sir.infected.size()
           && current_sir.age_group_proportions.size() == current_sir.recovered.size() && "There must be an equal number of age groups between"
                                                                          "age_group_proportions, susceptible, infected, and recovered!\n");
}

#endif //PANDEMIC_HOYA_2002_SIR_HPP
