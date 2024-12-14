def calculate_checksum(disk_map):
    # Parse the disk map into a list of IDs and free spaces
    block_map = []
    file_id = 0
    
    # Creating the initial blocks based on disk_map
    for char in disk_map:
        if char == '0':
            block_map.append(str(file_id))
            file_id += 1
        elif char == '.':
            block_map.append('.')
    
    # Calculate free positions
    free_positions = [i for i, x in enumerate(block_map) if x == '.']
    
    # Move blocks from the end to the leftmost free space
    for i in range(len(block_map)-1, -1, -1):
        if block_map[i] != '.':
            block_id = int(block_map[i])
            if free_positions:
                free_index = free_positions[0]
                block_map.pop(i)  # Remove from current position
                block_map.insert(free_index, str(block_id))  # Insert at the leftmost free space
            else:
                print(f"Warning: No free positions available for block ID {block_id} at index {i}")
    
    # Calculate checksum
    checksum = 0
    for idx, value in enumerate(block_map):
        if value != '.':
            checksum += idx * int(value)
    
    return checksum

# Read input from file 'input.txt'
try:
    with open('sample.txt', 'r') as file:
        disk_map = file.read().strip()
except FileNotFoundError:
    print("Error: File 'input.txt' not found.")
    exit()

# Calculate checksum
checksum = calculate_checksum(disk_map)
print(f"Resulting filesystem checksum: {checksum}")
