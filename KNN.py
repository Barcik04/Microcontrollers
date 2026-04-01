from sklearn.datasets import load_iris
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler

import math


def euclidean_distance(point_a, point_b):
    return math.sqrt(sum((point_a[i] - point_b[i]) ** 2 for i in range(len(point_a))))


iris = load_iris()

feature_matrix = iris.data # flower attributes
target_vector = iris.target # result (what flower is it)


############### to było dla mnie żebym sobie łatwiej mogł zobrazować jak wyglądają dane z iris ####################
# feature_names = iris.feature_names
# target_names = iris.target_names
#
# iris_df = pd.DataFrame(feature_matrix, columns=feature_names)
# iris_df["target"] = target_vector
# iris_df["species"] = [target_names[i] for i in target_vector]
#
# print(iris_df.head(10))
#
#




training_features, test_features, training_labels, test_labels = train_test_split(
    feature_matrix,
    target_vector,
    test_size=0.2,
    random_state=42
)



scaler = StandardScaler()
scaled_training_features = scaler.fit_transform(training_features)
scaled_test_features = scaler.transform(test_features)

# computes all predicted targets based on train data and returns predictions as an int table.
# train data is scaled_training_features (line 44) and data that we want to predict target of is scaled_test_features (line 45)
def knn_with_number_of_neighbours(k):
    all_predictions = []

    for test_row in scaled_test_features:
        distance_and_target_table = []

        for i in range(len(scaled_training_features)):
            training_row = scaled_training_features[i]
            training_target = training_labels[i]

            distance = euclidean_distance(test_row, training_row)
            distance_and_target_table.append((distance, training_target))

        distance_and_target_table.sort(key=lambda x: x[0])

        k_nearest_neighbors = distance_and_target_table[:k]

        target_count = {}

        for distance, target in k_nearest_neighbors:
            target = int(target)

            if target in target_count:
                target_count[target] += 1
            else:
                target_count[target] = 1

        predicted_target = max(target_count, key=target_count.get)
        all_predictions.append(predicted_target)

    return all_predictions




all_predictions = knn_with_number_of_neighbours(3)

all_predictions = [int(prediction) for prediction in all_predictions]
real_labels = [int(label) for label in test_labels]

print("Predictions:", all_predictions)
print("Real labels:", real_labels)
print()




correct_predictions = 0

for i in range(len(test_labels)):
    predicted_label = all_predictions[i]
    real_label = int(test_labels[i])

    if predicted_label == real_label:
        correct_predictions += 1

accuracy = (correct_predictions / len(test_labels)) * 100

print()
print(f"Correct predictions: {correct_predictions}/{len(test_labels)}")
print(f"Accuracy: {accuracy:.2f}%")