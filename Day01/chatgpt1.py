def read_input_from_file(file_path):
    left_list = []
    right_list = []
    with open(file_path, 'r') as file:
        for line in file:
            left, right = map(int, line.split())
            left_list.append(left)
            right_list.append(right)
    return left_list, right_list

def calculate_total_distance(left_list, right_list):
    # Sort both lists
    left_list.sort()
    right_list.sort()
    
    # Calculate the total distance
    total_distance = sum(abs(l - r) for l, r in zip(left_list, right_list))
    return total_distance

if __name__ == "__main__":
    file_path = "input.txt"  # Change this to your file's path
    left_list, right_list = read_input_from_file(file_path)
    total_distance = calculate_total_distance(left_list, right_list)
    print("Total distance:", total_distance)
