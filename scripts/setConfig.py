import json
import sys

with open('../.vscode/c_cpp_properties.json', 'r') as f:
    data = json.load(f)

data['configurations'][0]['compilerPath'] = sys.argv[1] + data['configurations'][0]['compilerPath']

with open('../.vscode/c_cpp_properties.json', 'w') as f:
    json.dump(data, f)