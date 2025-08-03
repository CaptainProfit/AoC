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
    unordered_set<string> layered_labels;

    public:
    static const int dx = 70;
    static const int dy = 50;
    static const int max_layer = 5;

    void SetEntityLayer(int layer, string label) {
        if (layers.size() <= layer) {
            layers.resize(layer+1);
        }
        svg::Point& pt = entities[label];
        layers[layer].emplace(label, &pt);
        pt.x = layer *(3*dx + 1);
    }

    void AllocateEntity(int layer, string label) {
        svg::Point& pt = entities[label];
        //average by ancestors
        pt.y = entities[scheme.exprs[label].left].y + entities[scheme.exprs[label].right].y;
        pt.y /= 2;
        pt.y = static_cast<int>(pt.y/dy)*dy;
        
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
        pt.x = static_cast<double>(static_cast<int>(pt.x));
        pt.y = static_cast<double>(static_cast<int>(pt.y));
        entities[label] = pt;
        assert(reserved_points.count(pt) == 0);
        reserved_points.insert(pt);
        layered_labels.insert(label);
    }
    
    svg::Point GetPoint(string label) {
        return entities[label];
    }

    void SortLayers() {
        
        for (int layer = 1; layer < layers.size(); layer++) {
            for (auto& [label, _] : layers[layer]) {
                AllocateEntity(layer, label);
            }
        }
    }

    void PlaceInputs() {
        for (int i = 0; i < 100; i++) {
            string label1 = Key('x', i);
            string label2 = Key('y', i);
            if (scheme.exprs.count(label1) == 0)
                break;

            svg::Point pt{0, dy*(3*i + 1)};
            AllocateEntity(0, label1, pt);

            pt += svg::Point{0, dy};
            AllocateEntity(0, label2, pt);
        }
    }

    void PlaceOutputs() {
        for (int i = 0; i < 100; i++) {
            string label = Key('z', i);
            if (scheme.exprs.count(label) == 0)
                break;
            svg::Point pt{dx*(3*max_layer + 1), dy*(3*i + 1)};
            AllocateEntity(max_layer, label, pt);
            layered_labels.insert(label);
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
        set<string> new_values;
        new_values = CalculateNextLayer1();
        int i = 0;
        do {
            i++;
            new_values = CalculateNextLayer2();
            for (auto& label : new_values) {
                if (layered_labels.count(label)) {
                    continue;
                }
                SetEntityLayer(i, label);
                layered_labels.emplace(label);
            }
            if (new_values.empty()) {
                break;
            }

        } while (!new_values.empty());
    }

public:
    Layout (Topology& scheme_) :
        scheme(scheme_) {
        for (auto &[key, _] : scheme.exprs) {
            entities.emplace(key, svg::Point());
        }
        
        //PlaceOutputs();
        PlaceInputs();
        LayoutNaive();
        SortLayers();
    }
    void Draw(svg::Document& doc);
};
