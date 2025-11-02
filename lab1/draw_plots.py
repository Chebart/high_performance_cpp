import argparse
from pathlib import Path
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os

def draw_plots(file_dir: str, out_dir: str):
    df = pd.read_csv(f"{file_dir}/ops_distr.csv")
    map_types = df['map_type'].unique()

    for op_name in df['operation'].unique():
        op_df = df[df['operation'] == op_name]

        for map_type in map_types:
            map_df = op_df[op_df['map_type'] == map_type]

            hash_types = map_df['hash_type'].unique()
            for hash_type in hash_types:
                hash_df = map_df[map_df['hash_type'] == hash_type]

                durations = hash_df['duration_us'].dropna()
                if durations.empty:
                    continue

                low, high = durations.quantile([0.01, 0.99])
                durations = durations[(durations >= low) & (durations <= high)]
                plt.figure(figsize=(10, 6))
                plt.hist(
                    durations, 
                    bins=6, 
                    alpha=0.7, 
                    weights=np.ones_like(durations) / len(durations),    
                    edgecolor='black',
                    linewidth=1.2,
                    color='skyblue'
                )
                plt.xlabel("Operation time")
                plt.ylabel("Probability")
                plt.title(f"{op_name} -  {hash_type}")
                filename = f"{out_dir}/{op_name}_{hash_type}.png"
                plt.savefig(filename)
                plt.close()

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--results', required=True, type=Path)
    parser.add_argument('--out', required=True, type=Path)
    args = parser.parse_args()
    draw_plots(args.results, args.out)

if __name__ == '__main__':
    main()