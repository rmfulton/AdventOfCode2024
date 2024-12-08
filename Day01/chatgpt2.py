from collections import Counter

def read_input_from_file(file_path):
    left_list = []
    right_list = []
    with open(file_path, 'r') as file:
        for line in file:
            left, right = map(int, line.split())
            left_list.append(left)
            right_list.append(right)
    return left_list, right_list

def calculate_similarity_score(left_list, right_list):
    # Count occurrences in the right list
    right_counts = Counter(right_list)
    
    # Calculate the similarity score
    similarity_score = sum(num * right_counts[num] for num in left_list)
    return similarity_score

if __name__ == "__main__":
    file_path = "sample.txt"  # Change this to your file's path
    left_list, right_list = read_input_from_file(file_path)
    similarity_score = calculate_similarity_score(left_list, right_list)
    print("Similarity score:", similarity_score)
