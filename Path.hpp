//
//  Path.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/3/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Path_hpp
#define Path_hpp

#include "Drawable.hpp"

#include "Coordinate.hpp"
#include "Line.hpp"
#include "Arc.hpp"

namespace nautical {
    class Path : public Drawable {
    public:
        Path() { }
        
        virtual ~Path() {
            for (std::vector<Node*>::iterator it = path.begin(); it != path.end(); it++) {
                delete *it;
            }
        }
        
        int getCount() {
            return (int)path.size();
        }
        
        Path & addLine(const Line & line) {
            path.push_back(new LineNode(line));
            return *this;
        }
        
        Path & addArc(const Arc & arc) {
            path.push_back(new ArcNode(arc));
            return *this;
        }
        
        Path & clear() {
            path.clear();
            return *this;
        }
        
        Coordinate getEndPoint() {
            if (path.size() > 0)
                return path.back()->getEndCoor();
            else
                return Coordinate();
        }
        
        void draw() const {
            for (std::vector<Node*>::const_iterator it = path.begin(); it != path.end(); it++) {
                (*it)->draw();
            }
        }
        
    private:
        struct Node : public Tagable {
            Node() { appendTag("PathNode"); }
            virtual bool intersectsNode(Node * p_node, std::vector<Coordinate> * p_intersections = nullptr) const = 0;
            virtual Coordinate getStartCoor() const = 0;
            virtual Coordinate getEndCoor() const = 0;
            virtual void draw() const = 0;
        };
        
        struct LineNode : public Node {
            Line line;
            
            LineNode(const Line & line) :
            line(line) { appendTag("LineNode"); }
            
            virtual bool intersectsNode(Node * p_node, std::vector<Coordinate> * p_intersections = nullptr) const {
                if (p_node->hasTag("LineNode")) {
                    return static_cast<LineNode*>(p_node)->line.intersectsLine(line, p_intersections);
                } else if (p_node->hasTag("ArcNode")) {
                    return static_cast<ArcNode*>(p_node)->arc.intersectsLine(line, p_intersections);
                } else {
                    return false;
                }
            }
            virtual Coordinate getStartCoor() const {
                return line.getCoor1();
            }
            virtual Coordinate getEndCoor() const {
                return line.getCoor2();
            }
            virtual void draw() const {
                GraphicsManager::drawLine(line);
            }
        };
        
        struct ArcNode : public Node {
            Arc arc;
            
            ArcNode(const Arc & arc) :
            arc(arc) { appendTag("ArcNode"); }
            virtual bool intersectsNode(Node * p_node, std::vector<Coordinate> * p_intersections = nullptr) const {
                if (p_node->hasTag("LineNode")) {
                    return arc.intersectsLine(static_cast<LineNode*>(p_node)->line, p_intersections);
                } else if (p_node->hasTag("ArcNode")) {
                    return arc.intersectsArc(static_cast<ArcNode*>(p_node)->arc, p_intersections);
                } else {
                    return false;
                }
            }
            virtual Coordinate getStartCoor() const {
                return arc.getStartCoor();
            }
            virtual Coordinate getEndCoor() const {
                return arc.getEndCoor();
            }
            virtual void draw() const {
                //TODO;
            }
        };
        
        std::vector<Node*> path;
    };
}

#endif /* Path_hpp */
