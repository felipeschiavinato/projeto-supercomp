import matplotlib.pyplot as plt

def plot_execution_times(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()

    graph_numbers = []
    execution_times = []
    for line in lines:
        parts = line.split()
        graph_number = int(parts[0].split('_')[1].split('.')[0])
        time = float(parts[1])
        
        graph_numbers.append(graph_number)
        execution_times.append(time)

    plt.plot(graph_numbers, execution_times, marker='o')
    plt.xlabel('Graph Number')
    plt.ylabel('Execution Time (seconds)')
    plt.title('Execution Time for Each Graph')
    plt.show()

    # Save graph in path=../results/sequencial_times{}.png

# Use the function like this:
plot_execution_times(r"../graphs/exhaustion_times.txt")
