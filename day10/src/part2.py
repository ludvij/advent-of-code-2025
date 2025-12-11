import pulp # type: ignore


input_path = 'inputs/day10'


def parse_input(text:str) -> dict:
	machines = []
	for line in text:
		parts = line.split(' ')
		machine = {
			'buttons' : [[int(y) for y in x[1:-1].split(',')] for x in parts[1:-1]],
			'joltage' : [int(x) for x in parts[-1][1:-1].split(',')]
		}
		machines.append(machine)

	return machines


def do_program(path:str) -> int:
	with open(path, 'r', encoding='utf-8') as f:
		text = [x.strip() for x in f.readlines()]

	machines = parse_input(text)
	res:int = 0
	for machine in machines:
		n = len(machine['joltage'])
		m = len(machine['buttons'])

		lp_problem = pulp.LpProblem("min_buttons", pulp.LpMinimize)

		lp_x_vars = [pulp.LpVariable(f'x_{i}', lowBound=0, cat='Integer') for i in range(m)]

		lp_problem += pulp.lpSum(lp_x_vars)

		for i in range(n):
			lp_constraints_expr = pulp.lpSum(lp_x_vars[j] for j in range(m) if i in machine['buttons'][j])
			lp_problem += lp_constraints_expr == machine['joltage'][i]

		lp_solver = pulp.PULP_CBC_CMD(msg=False)
		lp_problem.solve(lp_solver)

		if pulp.LpStatus[lp_problem.status] == 'Optimal':
			res += sum(int(pulp.value(x)) for x in lp_x_vars)

	return res
	

def main():
	if (test := do_program(f'{input_path}/test.txt')) != 33:
		print(f"test fails {test} != 33")
		return
	result = do_program(f'{input_path}/input.txt')

	print(f'[RESULT]: {result}')


if __name__ == '__main__':
	main()