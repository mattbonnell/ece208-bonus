""" A Python Class
A simple Python graph class, demonstrating the essential 
facts and functionalities of graphs.
"""


class Graph(object):

    def __init__(self, graph_dict=None):
        """ initializes a graph object 
            If no dictionary or None is given, 
            an empty dictionary will be used
        """
        if graph_dict == None:
            graph_dict = {}
        self.__graph_dict = graph_dict


    def vertex_cover_cnf(self, graph_dict=None, k=None, filename=None):
        result = []
        if graph_dict:
            graph = Graph(graph_dict=graph_dict)
            edges = graph.edges()
        else:
            edges = self.edges()
            while edges:
                edge = edges.pop()
                result.append(edge)
                edge_list = list(edge)
                first = edge_list[0]
                second = ""
                if len(edge_list) == 2:
                    second = edge_list[1]
                edges = [edge for edge in edges if not (first in edge or second in edge)]
        if k is not None and len(result) > k:
            print(f"Unable to find vertex cover of size at most {k}")
            return None
        if filename is not None:
            with open(filename, 'w') as output_file:
                for edge in result:
                    edge_list = list(edge)
                    first = edge_list[0]
                    if len(edge_list) == 2:
                        second = edge_list[1]
                        output_file.write(f"{first} {second} 0\n")
                    else:
                        output_file.write(f"{first} 0\n")
        return result



    def vertices(self):
        """ returns the vertices of a graph """
        return list(self.__graph_dict.keys())

    def edges(self):
        """ returns the edges of a graph """
        return self.__generate_edges()

    def add_vertex(self, vertex):
        """ If the vertex "vertex" is not in 
            self.__graph_dict, a key "vertex" with an empty
            list as a value is added to the dictionary. 
            Otherwise nothing has to be done. 
        """
        if vertex not in self.__graph_dict:
            self.__graph_dict[vertex] = []

    def add_edge(self, edge):
        """ assumes that edge is of type set, tuple or list; 
            between two vertices can be multiple edges! 
        """
        edge = set(edge)
        (vertex1, vertex2) = tuple(edge)
        if vertex1 in self.__graph_dict:
            self.__graph_dict[vertex1].append(vertex2)
        else:
            self.__graph_dict[vertex1] = [vertex2]

    def __generate_edges(self):
        """ A static method generating the edges of the 
            graph "graph". Edges are represented as sets 
            with one (a loop back to the vertex) or two 
            vertices 
        """
        edges = []
        for vertex in self.__graph_dict:
            for neighbour in self.__graph_dict[vertex]:
                if {neighbour, vertex} not in edges:
                    edges.append({vertex, neighbour})
        return edges

    def __str__(self):
        res = "vertices: "
        for k in self.__graph_dict:
            res += str(k) + " "
        res += "\nedges: "
        for edge in self.__generate_edges():
            res += str(edge) + " "
        return res


if __name__ == "__main__":

    g = { "a" : ["d"],
          "b" : ["c"],
          "c" : ["b", "c", "d", "e"],
          "d" : ["a", "c"],
          "e" : ["c"],
          "f" : ["d"]
        }


    graph = Graph(g)

    print("Computing minimal vertex cover")
    print(graph.vertex_cover_cnf(k=5, filename="output.cnf"))
  