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
            for (Iterator<Node*> * iterator = path.createIterator(); !iterator->complete(); iterator->next()) {
                delete iterator->current();
            }
        }
        
        int getCount() {
            return path.size();
        }
        
        bool addLine(Line line) {
            return path.insert(new LineNode(line));
        }
        
        bool addArc(Arc arc) {
            return path.insert(new ArcNode(arc));
        }
        
        bool clear() {
            return path.clear();
        }
        
        Coordinate getEndPoint() {
            Node * p_node = nullptr;
            if (path.getLastElement(&p_node)) {
                return p_node->getEndCoor();
            }
            return Coordinate();
        }
        
        void draw() const {
            for (Iterator<Node*> * iterator = path.createIterator(); !iterator->complete(); iterator->next()) {
                iterator->current()->draw();
            }
        }
        
    private:
        struct Node : public Tagable {
            Node() { appendTag("PathNode"); }
            virtual bool intersectsNode(Node * p_node, Queue<Coordinate> * p_intersections) const = 0;
            virtual Coordinate getStartCoor() const = 0;
            virtual Coordinate getEndCoor() const = 0;
            virtual void draw() const = 0;
        };
        
        struct LineNode : public Node {
            Line line;
            
            LineNode(Line line) :
            line(line) { appendTag("LineNode"); }
            
            virtual bool intersectsNode(Node * p_node, Queue<Coordinate> * p_intersections) const {
                if (p_node->hasTag("LineNode")) {
                    Coordinate intersection;
                    if (static_cast<LineNode*>(p_node)->line.intersectsLine(line, &intersection)) {
                        if (p_intersections)
                            p_intersections->insert(intersection);
                        return true;
                    } else {
                        return false;
                    }
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
            
            ArcNode(Arc arc) :
            arc(arc) { appendTag("ArcNode"); }
            virtual bool intersectsNode(Node * p_node, Queue<Coordinate> * p_intersections) const {
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
        
        LinkedList<Node*> path;
    };
}

#endif /* Path_hpp */
