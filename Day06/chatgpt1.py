def parse_map_from_file(file_path):
    with open(file_path, 'r') as file:
        map_lines = [line.strip() for line in file.readlines()]
    return parse_map(map_lines)

def parse_map(map_lines):
    grid = [list(line) for line in map_lines]
    directions = {'^': (-1, 0), '>': (0, 1), 'v': (1, 0), '<': (0, -1)}
    start_position = None
    start_direction = None

    # Find the guard's starting position and direction
    for i, row in enumerate(grid):
        for j, cell in enumerate(row):
            if cell in directions:
                start_position = (i, j)
                start_direction = directions[cell]
                grid[i][j] = '.'  # Replace guard's marker with empty space
                break
        if start_position:
            break

    return grid, start_position, start_direction

def turn_right(direction):
    # Define the right-turn logic
    return {
        (-1, 0): (0, 1),   # Up -> Right
        (0, 1): (1, 0),    # Right -> Down
        (1, 0): (0, -1),   # Down -> Left
        (0, -1): (-1, 0),  # Left -> Up
    }[direction]

def simulate_guard_path(grid, start_position, start_direction):
    rows, cols = len(grid), len(grid[0])
    visited_positions = set()
    position = start_position
    direction = start_direction

    while True:
        visited_positions.add(position)
        next_row, next_col = position[0] + direction[0], position[1] + direction[1]

        # Check if the guard is leaving the map
        if not (0 <= next_row < rows and 0 <= next_col < cols):
            break

        # Check the cell in front
        if grid[next_row][next_col] == '#':
            # Turn right if blocked
            direction = turn_right(direction)
        else:
            # Step forward
            position = (next_row, next_col)

    return visited_positions

if __name__ == "__main__":
    # Path to the input file
    file_path = "input.txt"  # Change this to your file's path
    
    grid, start_position, start_direction = parse_map_from_file(file_path)
    visited_positions = simulate_guard_path(grid, start_position, start_direction)
    print("Distinct positions visited:", len(visited_positions))
