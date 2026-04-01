from itertools import combinations


data = [
    [1, 1, 1, 1, 3, 1, 1],
    [1, 1, 1, 1, 3, 2, 1],
    [1, 1, 1, 3, 2, 1, 0],
    [1, 1, 1, 3, 3, 2, 1],
    [1, 1, 2, 1, 2, 1, 0],
    [1, 1, 2, 1, 2, 2, 1],
    [1, 1, 2, 2, 3, 1, 0],
    [1, 1, 2, 2, 4, 1, 1]
]



# rows = [row[:-1] for row in data]
# last_col = [col[-1] for col in data]
# columns = list(zip(*rows))
#
# print(rows)
# print(columns)
# print(last_col)
#
#
#
# def find_rule(values):
#     combs_columns = combs(len(columns))
#
#     found_rules = []
#
#     for i in combs_columns:
#         print("\n\n\n\n")
#         for row_idx in range(len(values)):
#
#             pattern = ["x"] * len(columns)
#
#             for col_idx in i:
#                 pattern[col_idx] = str(columns[col_idx][row_idx])
#
#             rule_key = "".join(pattern)
#             rule_value = last_col[row_idx]
#
#             matching_rows = []
#
#             for r in range(len(values)):
#                 match = True
#                 for col_idx in i:
#                     if columns[col_idx][r] != columns[col_idx][row_idx]:
#                         match = False
#                         break
#
#                 if match:
#                     matching_rows.append(r)
#
#             last_values = [last_col[r] for r in matching_rows]
#
#             if len(matching_rows) >= 2 and all(x == last_values[0] for x in last_values):
#
#                 rule = {rule_key: rule_value}
#
#                 if rule not in found_rules:
#                     found_rules.append(rule)
#
#     return found_rules
#
#
#
# def combs(n):
#     indexes = list(range(n))
#     result = []
#
#     for r in range(1, n + 1):
#         for combo in combinations(indexes, r):
#             result.append(list(combo))
#
#     return result
#
#
#
# print(find_rule(rows))



rows = [row[:-1] for row in data]
labels = [row[-1] for row in data]


def sequential(rows, labels, target):
    positives = []

    for i in range(len(labels)):
        if labels[i] == target:
            positives.append(i)

    for r in range(1, len(rows[0]) + 1):
        for cols in combinations(range(len(rows[0])), r):
            for pos in positives:

                values = []
                for col in cols:
                    values.append(rows[pos][col])

                covered = []

                for i in range(len(rows)):
                    match = True

                    for j in range(len(cols)):
                        if rows[i][cols[j]] != values[j]:
                            match = False
                            break

                    if match:
                        covered.append(i)

                good = True

                for i in covered:
                    if labels[i] != target:
                        good = False
                        break

                if good:
                    new_rows = []
                    new_labels = []

                    for k in range(len(rows)):
                        if k in covered and labels[k] == target:
                            continue
                        new_rows.append(rows[k])
                        new_labels.append(labels[k])

                    return (cols, values), covered, new_rows, new_labels

    return None, [], rows, labels


def find_all_rules(data, target):
    rows = [row[:-1] for row in data]
    labels = [row[-1] for row in data]

    all_rules = []

    while target in labels:
        rule, covered, rows, labels = sequential(rows, labels, target)

        if rule is None:
            break

        all_rules.append(rule)

    return all_rules


def print_rules(rules, target):
    for cols, values in rules:
        parts = []

        for i in range(len(cols)):
            parts.append(f"a{cols[i] + 1} = {values[i]}")

        print(" AND ".join(parts) + f" -> d = {target}")


rules_1 = find_all_rules(data, 1)
rules_0 = find_all_rules(data, 0)

print("Reguly dla d = 1:")
print_rules(rules_1, 1)

print("\nReguly dla d = 0:")
print_rules(rules_0, 0)