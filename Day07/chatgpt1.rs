use std::fs;

fn evaluate_expression(numbers: &[i64], operators: &[char]) -> i64 {
    let mut result = numbers[0];
    for (i, &op) in operators.iter().enumerate() {
        match op {
            '+' => result += numbers[i + 1],
            '*' => result *= numbers[i + 1],
            _ => panic!("Unknown operator"),
        }
    }
    result
}

fn generate_operators_combinations(length: usize) -> Vec<Vec<char>> {
    let mut combinations = Vec::new();
    let ops = vec!['+', '*'];

    // Generate all combinations of operators
    for i in 0..(1 << length) {
        let mut combination = Vec::new();
        for j in 0..length {
            if (i & (1 << j)) != 0 {
                combination.push(ops[1]);
            } else {
                combination.push(ops[0]);
            }
        }
        combinations.push(combination);
    }

    combinations
}

fn process_line(line: &str) -> Option<i64> {
    // Parse the target value and numbers
    let parts: Vec<&str> = line.split(": ").collect();
    let target = parts[0].parse::<i64>().expect("Failed to parse target value");
    let numbers: Vec<i64> = parts[1]
        .split_whitespace()
        .map(|n| n.parse().expect("Failed to parse number"))
        .collect();

    let num_operators = numbers.len() - 1;
    let operators_combinations = generate_operators_combinations(num_operators);

    for combination in operators_combinations {
        if evaluate_expression(&numbers, &combination) == target {
            return Some(target);
        }
    }

    None
}

fn main() {
    let input = fs::read_to_string("input.txt").expect("Failed to read input file");
    let mut total_calibration_result = 0;

    for line in input.lines() {
        if let Some(valid_value) = process_line(line) {
            total_calibration_result += valid_value;
        }
    }

    println!("Total calibration result: {}", total_calibration_result);
}
