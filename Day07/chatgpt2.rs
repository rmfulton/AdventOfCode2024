use std::fs;

fn evaluate_expression(numbers: &[i64], operators: &[char]) -> i64 {
    let mut result = numbers[0];
    for (i, &op) in operators.iter().enumerate() {
        match op {
            '+' => result += numbers[i + 1],
            '*' => result *= numbers[i + 1],
            '|' => {
                // Concatenation operator
                let concatenated = format!("{}{}", result, numbers[i + 1])
                    .parse::<i64>()
                    .expect("Failed to parse concatenated number");
                result = concatenated;
            }
            _ => panic!("Unknown operator"),
        }
    }
    result
}

fn generate_operators_combinations(length: usize) -> Vec<Vec<char>> {
    let mut combinations = Vec::new();
    let ops = vec!['+', '*', '|'];

    // Generate all combinations of operators
    let total_combinations = ops.len().pow(length as u32);
    for i in 0..total_combinations {
        let mut combination = Vec::new();
        let mut value = i;
        for _ in 0..length {
            combination.push(ops[value % ops.len()]);
            value /= ops.len();
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
