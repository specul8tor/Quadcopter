import json
import sys

with open('../.vscode/c_cpp_properties.json', 'r') as f:
    data = json.load(f)

data['configurations'][0]['compilerPath'] = sys.argv[1] + data['configurations'][0]['compilerPath']
data['configurations'][0]['includePath'][3] = sys.argv[1] + data['configurations'][0]['includePath'][3]
data['configurations'][0]['includePath'][4] = sys.argv[1] + data['configurations'][0]['includePath'][4]

with open('../.vscode/c_cpp_properties.json', 'w') as f:
    json.dump(data, f)