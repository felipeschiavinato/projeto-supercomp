import matplotlib.pyplot as plt
import numpy as np

def plot_execution_times():

    file_names = ['../results/sequencial/times/sequential_times', '../results/mpi/times/parallel_times', '../results/openmp/times/openmp_times']
    labels = ['Sequencial', 'MPI', 'OpenMP']
    alg_data = []
    for filename in file_names:
        s_data = []  # Initialize s_data as a list
        for i in range(0, 10):
            with open(filename + f'_{i}.txt', 'r') as file:
                lines = file.readlines()

            graph_numbers = []
            execution_times = []
            for line in lines:
                parts = line.split()
                graph_number = int(parts[0].split('_')[1].split('.')[0])
                time = float(parts[1])
                
                graph_numbers.append(graph_number)
                execution_times.append(time)

            execution_times = np.array(execution_times)
            s_data.append(execution_times)  # Append to the list

        avg = np.mean(np.array(s_data), axis=0)  # Convert list to NumPy array before calculating mean
        alg_data.append(avg)


    plt.figure(figsize=(10, 5))
    plt.plot(alg_data[0], label=labels[0])
    plt.plot(alg_data[1], label=labels[1])
    plt.plot(alg_data[2], label=labels[2])
    plt.legend()
    plt.xlabel('n_vertices')
    plt.ylabel('Tempo de execução (s)')
    plt.title('Tempo de execução dos algoritmos')
    plt.savefig('../results/execution_times.png')
    plt.show()

plot_execution_times()

    

    
