#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "topology.hpp"
#include "svg.hpp"
string Key(char c, int i);

class Layout {
    Topology& scheme;
    
    using Layer = map<string, svg::Point*>;
    map<string, svg::Point> entities;
    vector<Layer> layers;
    set<svg::Point> reserved_points;
    unordered_set<string> allocated_labels;
    unordered_set<string> layered_labels;

    public:
    static const int dx = 60;
    static const int dy = 50;
    static const int max_layer = 5;
    const int depth = 50;
    bool SetEntityLayer(int layer, string label) {
        if (layered_labels.count(label) > 0) 
            return false;
        if (layers.size() <= layer) {
            layers.resize(layer+1);
        }
        svg::Point& pt = entities[label];
        layers[layer].emplace(label, &pt);
        pt = {0, 0};
        layered_labels.insert(label);
        return true;
    }

    void AllocateEntity(int layer, string label) {
        svg::Point& pt = entities[label];
        //average by ancestors
        int cnt = 0;
        pt.x = 3*layer*dx;
        if (allocated_labels.count(scheme.exprs[label].left) == 0)
            return;
        if (allocated_labels.count(scheme.exprs[label].right) == 0)
            return;
        pt.y = entities[scheme.exprs[label].left].y + entities[scheme.exprs[label].right].y;
        pt.y /= 2;
        pt.y = static_cast<int>(pt.y/dy)*dy;
        pt.y += layer*dx/max_layer;
        svg::Point new_pt(pt);
        int i = 0;
        while (reserved_points.count(new_pt) != 0) {
            new_pt = pt + svg::Point{0, dy*i};
            if (reserved_points.count(new_pt) == 0)
                break;
            new_pt = pt - svg::Point{0, dy*i};
            if (reserved_points.count(new_pt) == 0)
                break;
            i++;
        }
        AllocateEntity(layer, label, new_pt);
    }

    void AllocateEntity(int layer, string label, svg::Point pt) {
        SetEntityLayer(layer, label);
        //pt.x = static_cast<double>(static_cast<int>(pt.x));
        //pt.x ;
        //pt.y = static_cast<double>(static_cast<int>(pt.y));
        entities[label] = pt;
        assert(reserved_points.count(pt) == 0);
        reserved_points.insert(pt);
        allocated_labels.insert(label);
    }
    
    svg::Point GetPoint(string label) {
        return entities[label];
    }

    void AllocateLayers() {
        for (int layer = 1; layer < layers.size(); layer++) {
            for (auto& [label, _] : layers[layer]) {
                if (allocated_labels.count (label) == 0)
                    AllocateEntity(layer, label);
            }
        }
    }

    void PlaceInput(int i) {
        string label1 = Key('x', i);
        string label2 = Key('y', i);
        if (scheme.exprs.count(label1) == 0)
            return;

        svg::Point pt{0, dy*(3*i + 1)};
        AllocateEntity(0, label1, pt);

        pt += svg::Point{dx/2, dy};
        AllocateEntity(0, label2, pt);
    }

    void PlaceInputs(int max = 100) {
        for (int i = 0; i < max; i++) {
            PlaceInput(i);
        }
    }

    void PlaceOutputs(int max = 100) {
        for (int i = 0; i < max; i++) {
            string label = Key('z', i);
            if (scheme.exprs.count(label) == 0)
                break;
            svg::Point pt{dx*(3*max_layer + 1), dy*(3*i + 1)};
            AllocateEntity(max_layer, label, pt);
        }
    }

    set<string> CalculatePrevLayer() {
        set<string> inputs;
        for (auto& key : layered_labels ) {
            string left = scheme.exprs[key].left;
            string right = scheme.exprs[key].right;
            if (layered_labels.count(left) == 0)
                if (layered_labels.count(scheme.exprs[left].left) == 0 ||
                    layered_labels.count(scheme.exprs[left].right) == 0)
                        inputs.insert(left);
            if (layered_labels.count(right) == 0)
                if (layered_labels.count(scheme.exprs[right].left) == 0 ||
                    layered_labels.count(scheme.exprs[right].right) == 0)
                        inputs.insert(right);
        }
        return inputs;
    }

    set<string> CalculateNextLayer1() {
        set<string> inputs;
        for (auto& key : layered_labels )
            for (auto& next : scheme.expansions[key])
                if (layered_labels.count(next) == 0)
                    inputs.insert(next);
        return inputs;
    }

    set<string> CalculateNextLayer2() {
        set<string> inputs;
        for (auto& key : layered_labels )
            for (auto& next : scheme.expansions[key]) {
                if (layered_labels.count(scheme.exprs[next].left) == 0)
                    continue;
                if (layered_labels.count(scheme.exprs[next].right) == 0)
                    continue;
                if (next == "none" || next == "var") 
                    continue;
                if (layered_labels.count(next) == 0)
                    inputs.insert(next);
            }        
        return inputs;
    }

    void LayoutNaive() {
        //PlaceOutputs();
        PlaceInputs(depth);
        set<string> new_values;
        new_values = CalculateNextLayer1();
        int i = 0;
        do {
            i++;
            new_values = CalculateNextLayer2();
            for (auto& label : new_values) {
                if (allocated_labels.count(label)) {
                    continue;
                }
                SetEntityLayer(i, label);
            }
            if (new_values.empty()) {
                break;
            }
        } while (!new_values.empty());
        AllocateLayers();
    }
    
    void Layout2() {
        PlaceOutputs(depth);
        for (int i = 0; i < depth; i++) {
            PlaceInput(i);
            int level = 0;
            set<string> new_values;
            //new_values = CalculateNextLayer2();
            do {
                level++;
                new_values = CalculateNextLayer2();
                for (auto& label : new_values) {
                    if (layered_labels.count(label)) {
                        continue;
                    }
                    SetEntityLayer(level, label);
                }
            } while (!new_values.empty());
            AllocateLayers();
        }
    }

    void SortLayer(int layer) {
        for (int i = 0; i < depth; i++) {
            string key = Key('x', i);
            if (scheme.expansions.count(key) == 0) {
                break;
            }
            auto it = scheme.expansions[key].begin();
            auto it2 = it;
            it2++;
            if (it2 == scheme.expansions[key].end())
                continue;
            auto left = *it;
            auto right = *it2;
            if (left[0] == 'z' || right[0] == 'z') {
                continue;
            }
            if (scheme.exprs[left].op != "AND") {
                auto tmp = entities[left];
                entities[left] = entities[right];
                entities[right] = tmp;
            }
        }
    };

    void AlignLayer(int layer, set<string> filter) {
        map<int, string> labels;
        for (auto& [label, ppt] : layers[layer]) {
            if (scheme.exprs[label].op == "") {
                assert(0);
            }
            if (filter.count(label) == 0)
                labels[ppt->y] = label;
            
        }
        int i = 1;
        for (auto it = labels.begin(); it != labels.end(); it++) {
            entities[it->second].y = 3*i*dy + 30;
            i++;
        }
    };

public:
    Layout (Topology& scheme_) :
        scheme(scheme_) {
        for (auto &[key, _] : scheme.exprs) {
            entities.emplace(key, svg::Point());
        }
        auto& x = entities["z06"];
        Layout2();
        SortLayer(1);
        AlignLayer(2, set{"gmh"s, "jmq"s});
        //AligntLayer(2);
    }
    void Draw(svg::Document& doc);
};
