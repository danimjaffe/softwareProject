# import mykmeanssp as kn
# from math import sqrt
# import argparse
# import numpy as np
# import pandas as pd

# np.random.seed(0)


# def get_data(filename):
#     df = pd.read_csv(filename, index_col=0, header=None)
#     return df


# def output_results(results, centroids_idx, n_dimensions):
#     print(','.join(centroids_idx))

#     new_line = "\n"
#     res_str = [f'{res:.4f}{new_line if (i + 1) % n_dimensions == 0 else ","}'
#                for i, res in enumerate(results)]
#     res_str = ''.join(res_str)
#     print(res_str)


# def flatten_data(matrix):
#     data = []
#     for x in matrix:
#         for y in x:
#             data.append(y)
#     return data


# class KmeansPP:
#     def __init__(self, data, n_dimensions, k, max_iter=200):
#         self.data = data
#         self.data_arr = data.to_numpy()
#         self.n_dimensions = n_dimensions
#         self.k = k
#         self.max_iter = max_iter
#         rand_idx = np.random.choice(self.data.index)
#         self.centroids_idx = [str(int(rand_idx))]
#         self.centroids = [self.data.loc[rand_idx].to_numpy()]

#     def initialize_centroids(self):
#         for i in range(1, self.k):
#             self.calc_centroid()
#         return self.centroids

#     def calc_centroid(self):
#         prob_func = self.calc_prob()
#         rand_idx = np.random.choice(self.data.index, p=prob_func)
#         self.centroids_idx.append(str(int(rand_idx)))
#         self.centroids.append(self.data.loc[rand_idx])

#     def calc_prob(self):
#         dist_sum = 0
#         min_dist_arr = np.empty(shape=len(self.data))
#         for i, point in enumerate(self.data_arr):
#             min_dist = euclidean_distance(point, self.centroids[0])
#             for centroid in self.centroids:
#                 curr_dist = euclidean_distance(point, centroid)
#                 if curr_dist < min_dist:
#                     min_dist = curr_dist
#             min_dist_arr[i] = min_dist
#             dist_sum += min_dist
#         return min_dist_arr / dist_sum


# ####################
# # Helper Functions #
# ####################

# def euclidean_norm(u, v):
#     squared_dist = euclidean_distance(u, v)
#     norm = sqrt(squared_dist)
#     return norm


# def euclidean_distance(u, v):
#     assert len(v) == len(u)
#     euclidean_dist = 0
#     for dim in range(len(u)):
#         euclidean_dist += pow(u[dim] - v[dim], 2)
#     return euclidean_dist


# def validate_input(k, max_iter, n_rows, eps):
#     assert (n_rows > k > 1)
#     assert (max_iter > 0)
#     assert (eps >= 0)


# class ArgumentParser(argparse.ArgumentParser):
#     def error(self, message):
#         self.exit(2)


# def main():
#     try:
#         parser = ArgumentParser()
#         parser.add_argument("k", type=int)
#         parser.add_argument("goal", type=str)
#         parser.add_argument("file_name", type=str)
#         args = parser.parse_args()
#         k = args.k
#         goal = args.goal
#         file_name = args.file_name
#         data = get_data(file_name)
#         validate_input(k, goal, file_name)
#     except:
#         raise SystemExit('Invalid Input!')

#     try:
#         kmeans_pp = KmeansPP(data, n_dimensions, k, max_iter=300)
#         centroids_idx = kmeans_pp.centroids_idx
#         data = flatten_data(kmeans_pp.data_arr)
#         centroids = kmeans_pp.initialize_centroids()
#         centroids = flatten_data(centroids)
#         result = kn.fit(k, max_iter, eps, n_dimensions, number_of_rows, centroids, data)
#         output_results(result, centroids_idx, n_dimensions)
#     except:
#         raise SystemExit('An Error Has Occurred')


# if __name__ == '__main__':
#     main()
