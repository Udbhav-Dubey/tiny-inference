# data_set

This folder contains the extracted model weights and biases, produced by running the `.pth` parser (`src/pth_converter.cpp`) on a PyTorch checkpoint.

The parser reads the `.pth` file, walks the pickle opcode stream, and writes each tensor out as a plain text file here. The inference engine reads directly from these files at runtime — no Python, no PyTorch needed after extraction.

---

## File format

### `weight{n}.txt`

```
<rows> <cols>
v0 v1 v2 ... vN
```

First line is the tensor shape. Remaining values are the weights in row-major order, space-separated. `rows` = output features, `cols` = input features — matching PyTorch's `nn.Linear` convention.

### `bias{n}.txt`

```
<rows> <cols>
v0 v1 v2 ... vN
```

Same format. For a bias vector of size `K`, this is stored as a `1 × K` tensor.

The numeric suffix `n` in the filename corresponds to the layer index as read from the model metadata. Even indices are weights, odd indices are biases — the inference engine asserts this ordering at runtime.

---

## Files in this folder

| File | Shape | Description |
|---|---|---|
| `weight0.txt` | 128 × 784 | Weights for Linear layer 0 (input → hidden) |
| `bias0.txt` | 1 × 128 | Biases for Linear layer 0 |
| `weight2.txt` | 10 × 128 | Weights for Linear layer 1 (hidden → output) |
| `bias2.txt` | 1 × 10 | Biases for Linear layer 1 |
| `metadata.txt` | — | Layer ordering, shapes, and types — read by the engine to construct the network at runtime |

---

## metadata.txt

Describes the layer sequence the engine will build. Each entry specifies the tensor type (`weight` or `bias`), its index number, and its shape. The engine iterates this file to construct the `Linear → ReLU → ... → Linear` sequence — no architecture is hardcoded.

---

## Regenerating

To re-extract from a different `.pth` checkpoint, run the binary and select option `2 : update the model`. The parser will overwrite these files with the new weights.