# find base parameter of chosen sequence
# by building suffix tree
# (tasks no.4)
import argparse
from pyvis.network import Network

class DynamicSystem:
    data_quantized = []
    def __init__(self, data, quantization_levels: int, log_data: str | None = None):
        self.data = data
        self.quantization_levels = quantization_levels
        self.quantization(log_data)
    
    def quantization(self, log_data: str | None = None):
        quantization_step = abs(max(self.data) - min(self.data)) / self.quantization_levels
        print('Quantization step =', quantization_step)
        
        # quantizing current data
        for value in self.data:
            self.data_quantized.append(str(round(value / quantization_step) * quantization_step))
        
        # logging quantized sequence
        if log_data:
            with open(log_data, 'w') as res:
                for val in self.data_quantized:
                    res.write(val + '    ')
    
    def path_length_count(self, edge, graph_edges):
        text_length = 0
        while edge[0] != 0:
            edge = tuple(filter(lambda x: x[1] == edge[0], graph_edges.keys()))[0]
            text_length += len(graph_edges[edge])
        return text_length

    def find_base_parameter(self, plot_suffix_tree):
        try:
            import lib.libsuffixtree
            suffix_tree = lib.libsuffixtree.SuffixTree(self.data_quantized)
            graph_edges = suffix_tree.get_edges()

            new_dict = {key: value for (key, value) in graph_edges.items() if value[-1] == '$'}

            bp = 0
            for edge in new_dict:
                tmp = self.path_length_count(edge, graph_edges)
                if tmp > bp:
                    bp = tmp
            bp += 1

            if plot_suffix_tree:
                # adj = [*range(0, len(graph_edges) + 1)]
                # nodes_labels = [str(x) for x in adj]

                # net = Network(height="1000px", width="100%",
                #             bgcolor="#222222", font_color="white")
                # net.barnes_hut()
                # net.add_nodes(
                #     list(adj),
                #     label=nodes_labels
                # )

                # for edge, value in graph_edges.items():
                #     net.add_edge(edge[0], edge[1], label=str(value))

                # net.show('graph.html')
                print(len(graph_edges))
            return bp
        except ImportError:
            print('No libsuffixtree. Please copy the lib to this directory')
            return None
      
def main():
    parser = argparse.ArgumentParser(description='Find base parameter of the dynamic system.')
    parser.add_argument('-d', '--data-file', required=True, help='Path to the file with sequence')
    parser.add_argument('-q', '--quant-levels', required=True, help='Number of quantization levels')
    parser.add_argument('-l', '--log-data', help='Log quantized sequence')
    parser.add_argument('--plot-suffix-tree', action='store_true', help='Visualize suffix tree graph')
    args = parser.parse_args()

    source_file = args.data_file
    quant_levels = int(args.quant_levels)
    log_data = args.log_data
    plot_suffix_tree = args.plot_suffix_tree
    
    data = []
    
    # reading data from source file and pick supporting metrics
    try:
        with open(source_file) as source:
            for line in source:
                data.append(float(line))

        ds = DynamicSystem(data, quant_levels, log_data)
        # counting base prameter
        bp = ds.find_base_parameter(plot_suffix_tree)
        print('Base parameter for current system =', bp)
    except FileNotFoundError:
        print('File was not found in the current directory')
    except ValueError:
        print('Error reading from file')
    
if __name__ == '__main__':
    main()