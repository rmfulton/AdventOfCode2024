def parse_map_from_file(file_path):
    with open(file_path, 'r') as file:
        map_lines = [line.strip() for line in file.readlines()]
    return parse_map(map_lines)

def parse_map(map_lines):
    grid = [list(line) for line in map_lines]
    directions = {'^': (-1, 0), '>': (0, 1), 'v': (1, 0), '<': (0, -1)}
    start_position = None
    start_direction = None

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
    return {
        (-1, 0): (0, 1),   # Up -> Right
        (0, 1): (1, 0),    # Right -> Down
        (1, 0): (0, -1),   # Down -> Left
        (0, -1): (-1, 0),  # Left -> Up
    }[direction]

def simulate_with_obstruction(grid, start_position, start_direction, obstruction):
    rows, cols = len(grid), len(grid[0])
    visited = set()
    position = start_position
    direction = start_direction
    current_grid = [row[:] for row in grid]
    current_grid[obstruction[0]][obstruction[1]] = '#'

    while True:
        if (position, direction) in visited:
            return True  # A loop is detected
        visited.add((position, direction))

        next_row, next_col = position[0] + direction[0], position[1] + direction[1]

        if not (0 <= next_row < rows and 0 <= next_col < cols):
            return False  # Guard leaves the map

        if current_grid[next_row][next_col] == '#':
            direction = turn_right(direction)
        else:
            position = (next_row, next_col)

def find_valid_obstructions(grid, start_position, start_direction):
    rows, cols = len(grid), len(grid[0])
    valid_positions = 0

    for r in range(rows):
        for c in range(cols):
            if (r, c) == start_position or grid[r][c] == '#':
                continue  # Cannot place an obstruction here

            if simulate_with_obstruction(grid, start_position, start_direction, (r, c)):
                valid_positions += 1

    return valid_positions

if __name__ == "__main__":
    file_path = "input.txt"  # Replace with your file path
    grid, start_position, start_direction = parse_map_from_file(file_path)
    result = find_valid_obstructions(grid, start_position, start_direction)
    print("Number of valid positions for obstruction:", result)
