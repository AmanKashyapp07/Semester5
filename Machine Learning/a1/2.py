import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.datasets import load_iris

def main():
    # Task i & ii: Load the Iris dataset
    iris = load_iris()
    
    # Task iii: Convert the dataset into a Pandas DataFrame
    df = pd.DataFrame(data=iris.data, columns=iris.feature_names)
    
    # Add target class labels for clarity in visualization
    df['target'] = iris.target
    df['species'] = df['target'].map({i: name for i, name in enumerate(iris.target_names)})

    # Task iv: Display the first five rows of the dataset
    print("=" * 60)
    print("Task iv: First 5 rows of the dataset:")
    print("=" * 60)
    print(df.head())
    print("\n")

    # Task v: Determine number of samples and features
    num_samples = df.shape[0]
    num_features = len(iris.feature_names)
    print("=" * 60)
    print("Task v: Dataset Dimensions:")
    print("=" * 60)
    print(f"Number of samples:  {num_samples}")
    print(f"Number of features: {num_features}")
    print("\n")

    # Task vi: Check for missing values
    missing_values = df[iris.feature_names].isnull().sum()
    print("=" * 60)
    print("Task vi: Missing Values per Feature:")
    print("=" * 60)
    print(missing_values)
    print(f"Total missing values: {missing_values.sum()}")
    print("\n")

    # Task vii: Calculate statistical measures for each feature
    features_df = df[iris.feature_names]
    stats_df = pd.DataFrame({
        'Mean': features_df.mean(),
        'Median': features_df.median(),
        'Max Value': features_df.max(),
        'Min Value': features_df.min()
    })
    print("=" * 60)
    print("Task vii: Statistical Measures per Feature:")
    print("=" * 60)
    print(stats_df)
    print("\n")

    # (a) Histograms for each feature
    fig, axes = plt.subplots(2, 2, figsize=(14, 10))
    fig.suptitle('Iris Dataset Exploratory Data Analysis & Visualizations', fontsize=16, fontweight='bold')

    feature_colors = ['crimson', 'forestgreen', 'darkorange', 'royalblue']
    for idx, feature in enumerate(iris.feature_names):
        ax = axes[idx // 2, idx % 2]
        df[feature].hist(ax=ax, bins=15, color=feature_colors[idx], edgecolor='black', alpha=0.7)
        ax.set_title(f'Histogram of {feature}')
        ax.set_xlabel(feature)
        ax.set_ylabel('Frequency')
        ax.grid(True, linestyle='--', alpha=0.5)

    plt.tight_layout(rect=[0, 0, 1, 0.96])
    plot_path_a = '/Users/amankashyap/Documents/Semester-5/Machine Learning/question2_histograms.png'
    plt.savefig(plot_path_a, dpi=300)
    plt.close()

    # (b) Scatter Plot and (c) Box Plot
    fig, axes = plt.subplots(1, 2, figsize=(16, 6))
    fig.suptitle('Iris Dataset Feature Comparisons', fontsize=16, fontweight='bold')

    # Scatter Plot: Petal Length vs Petal Width by Species
    ax_scatter = axes[0]
    colors = {'setosa': 'crimson', 'versicolor': 'forestgreen', 'virginica': 'royalblue'}
    for species, group in df.groupby('species'):
        ax_scatter.scatter(group['petal length (cm)'], group['petal width (cm)'], 
                           label=species, color=colors[species], alpha=0.85, edgecolors='k', s=50)
    ax_scatter.set_xlabel('Petal Length (cm)')
    ax_scatter.set_ylabel('Petal Width (cm)')
    ax_scatter.set_title('(b) Scatter Plot: Petal Length vs Petal Width')
    ax_scatter.legend(title='Species')
    ax_scatter.grid(True, linestyle='--', alpha=0.5)

    # Box Plot of features
    ax_box = axes[1]
    df[iris.feature_names].boxplot(ax=ax_box, patch_artist=True, 
                                   boxprops=dict(facecolor='lightblue', color='blue'))
    ax_box.set_title('(c) Box Plot of Features')
    ax_box.set_ylabel('Length / Width (cm)')
    ax_box.grid(True, linestyle='--', alpha=0.5)

    plt.tight_layout(rect=[0, 0, 1, 0.95])
    plot_path_bc = '/Users/amankashyap/Documents/Semester-5/Machine Learning/question2_scatter_boxplot.png'
    plt.savefig(plot_path_bc, dpi=300)
    print(f"Visualizations saved successfully to:\n - {plot_path_a}\n - {plot_path_bc}")
    plt.close()

    # Task ix: Print observations
    print("\n" + "=" * 60)
    print("Task ix: Three Key Observations:")
    print("=" * 60)
    print("1. Linearly Separable Species: Setosa is easily separable from Versicolor and Virginica based on petal length and width.")
    print("2. High Feature Correlation: Petal length and petal width display a strong positive linear correlation.")
    print("3. Outlier Presence: Sepal width exhibits mild outliers on both upper and lower tails, whereas other features show a clean continuous distribution without significant outliers.")

if __name__ == '__main__':
    main()
