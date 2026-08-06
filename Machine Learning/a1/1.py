import numpy as np
import matplotlib.pyplot as plt
from scipy.spatial.distance import pdist

def experiment_distances():
    # Dimensions d = 2^0, 2^1, ..., 2^10
    dimensions = [2**i for i in range(11)]
    num_points = 100

    # Lists to store statistics
    l2_sq_means = []
    l2_sq_stds = []
    l1_means = []
    l1_stds = []

    # Set seed for reproducibility
    np.random.seed(42)

    for d in dimensions:
        # Sample 100 points uniformly from unit cube [0, 1]^d
        points = np.random.rand(num_points, d)

        # 1. Squared Euclidean distance between all pairs
        # scipy pdist 'sqeuclidean' computes sum_j (x_j - y_j)^2
        l2_sq_dists = pdist(points, metric='sqeuclidean')
        
        # 2. L1 distance between all pairs
        # scipy pdist 'cityblock' computes sum_j |x_j - y_j|
        l1_dists = pdist(points, metric='cityblock')

        # Record mean and standard deviation
        l2_sq_means.append(np.mean(l2_sq_dists))
        l2_sq_stds.append(np.std(l2_sq_dists))

        l1_means.append(np.mean(l1_dists))
        l1_stds.append(np.std(l1_dists))

    # Print summary table
    print(f"{'Dimension (d)':<15} | {'L2 Sq Mean':<12} | {'L2 Sq Std':<12} | {'L1 Mean':<12} | {'L1 Std':<12}")
    print("-" * 75)
    for i, d in enumerate(dimensions):
        print(f"{d:<15} | {l2_sq_means[i]:<12.4f} | {l2_sq_stds[i]:<12.4f} | {l1_means[i]:<12.4f} | {l1_stds[i]:<12.4f}")

    # Plotting
    fig, axes = plt.subplots(2, 2, figsize=(14, 10))
    fig.suptitle('Pairwise Distance Statistics as a Function of Dimension $d$', fontsize=16, fontweight='bold')

    # Subplot 1: Average Squared Euclidean Distance
    axes[0, 0].plot(dimensions, l2_sq_means, 'o-', color='crimson', linewidth=2, label='Mean $L_2^2$')
    axes[0, 0].set_xscale('log', base=2)
    axes[0, 0].set_xlabel('Dimension $d$ (log scale)')
    axes[0, 0].set_ylabel('Average Distance')
    axes[0, 0].set_title('Average Squared Euclidean ($L_2^2$) Distance')
    axes[0, 0].grid(True, which="both", ls="--", alpha=0.6)
    axes[0, 0].legend()

    # Subplot 2: Std Dev of Squared Euclidean Distance
    axes[0, 1].plot(dimensions, l2_sq_stds, 's--', color='darkred', linewidth=2, label='Std $L_2^2$')
    axes[0, 1].set_xscale('log', base=2)
    axes[0, 1].set_xlabel('Dimension $d$ (log scale)')
    axes[0, 1].set_ylabel('Standard Deviation')
    axes[0, 1].set_title('Standard Deviation of $L_2^2$ Distance')
    axes[0, 1].grid(True, which="both", ls="--", alpha=0.6)
    axes[0, 1].legend()

    # Subplot 3: Average L1 Distance
    axes[1, 0].plot(dimensions, l1_means, 'o-', color='royalblue', linewidth=2, label='Mean $L_1$')
    axes[1, 0].set_xscale('log', base=2)
    
    axes[1, 0].set_xlabel('Dimension $d$ (log scale)')
    axes[1, 0].set_ylabel('Average Distance')
    axes[1, 0].set_title('Average $L_1$ Distance')
    axes[1, 0].grid(True, which="both", ls="--", alpha=0.6)
    axes[1, 0].legend()

    # Subplot 4: Std Dev of L1 Distance
    axes[1, 1].plot(dimensions, l1_stds, 's--', color='navy', linewidth=2, label='Std $L_1$')
    axes[1, 1].set_xscale('log', base=2)
    axes[1, 1].set_xlabel('Dimension $d$ (log scale)')
    axes[1, 1].set_ylabel('Standard Deviation')
    axes[1, 1].set_title('Standard Deviation of $L_1$ Distance')
    axes[1, 1].grid(True, which="both", ls="--", alpha=0.6)
    axes[1, 1].legend()

    plt.tight_layout()
    plot_path = '/Users/amankashyap/Documents/Semester-5/Machine Learning/question1_plot.png'
    plt.savefig(plot_path, dpi=300)
    print(f"\nPlot saved successfully to: {plot_path}")
    plt.close()

if __name__ == '__main__':
    experiment_distances()
