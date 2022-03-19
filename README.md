# HTML Code Manipulation Tool

- Program which allows users to format badly formatted XHTML 1.1 code, add or delete different HTML tags and modify block styles.
- Users will feed the necessary inputs to the program through the 'input.html' and 'commands.in' files.
- Uses a generic implementation for Singly Linked Lists and a specific one for HTML Trees.

## The structure of an HTML tree

The HTML code given as input will be stored as a tree in which every node will represent a tag. The exact structure of the HTML tree goes as follows:

- The root node, also reffered to as the `null node`, is the only node which has no corresponding HTML tag. It's children consist of only `<html>` nodes.
- The `<html>` node can be seen as the root of a single html file. The `null node` exists only as a way to group up different HTML trees into the same data structure.
- Each `<html>` node has exactly 2 children: the `<head>` and `<body>` tags.
- Internal nodes represent tags that also contain other tags.
- Leaf nodes correspond to tags which contain no other tags.

The ID's of nodes are specified as follows:

- The `<head>` node has ID 1.
- The `<body>` node has ID 2.
- The rest of the nodes follow this rule:
  - If the parent's ID is `X.Y.Z`, then the current node's ID is `X.Y.Z.<index of child in parent's children list>`

For example, the following code:

```
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
  <title>Page title</title>
</head>
<body>
  <div class="class1"><h1>This is a big text</h1></div>
  <div>
    <p style="color: red;font-size: 50px;" class="class2">Red and big font</p>
    <ul>
      <li>item 1</li>
      <li>item 2</li>
      <li>item 3</li>
    </ul>
    <p class="class2">simple text</p>
  </div>
  <div>
    <img style="float: right;border: 1px;"
      class="imageClass"
      src="../img/purple.png" alt="this is an image"
      width="500" height="600" />
  </div>
</body>
</html>
```

Has the following corresponding HTML tree:

![HTML Tree image](https://github.com/cezar-dan/html-code-manipulation-tool/blob/main/tree_ids.png)


## Input Files

The user will have to create two input files, in order for the program to properly run:
- `input.html` - contains the poorly formatted html code.
- `commands.in`
  - the first line: `n` - an int representing the number of commands given.
  - the following `n` lines: the input commands given.

The following restrictions have to be met regarding the input files, otherwise the program might experience undefined behaviour:
- There won't be lines longer than 100 * 1024 characters.
- The HTML code provided has to be valid.
- There won't be comments in the HTML code.
- An HTML tag will have no more than one class.
- No HTML tag will contain the `id` attribute.
- Only leaf nodes may contain useful information.
- There will be no `<script>` nodes containing Javascript code.
- There will be no `<style>` tags containing CSS code.

## Valid Commands

This is a list of valid commands that can be used in the `commands.in` file:
- `format`:
  - Correctly formats poorly indented code.
  - The number of tabs is given by the node's level in the HTML tree.

- `add ID=<PARENT_ID> tagHTML="<tagHTML>"`
  - Adds a new child to the node with the given ID.
  - This node will be added at the end of the parent's node child list.

- `deleteRecursively selector="<CSS_Selector>"`
  - Deletes all nodes in the HTML tree which satisfy the given CSS selector.
  - If a node which satisfies the CSS selector has children, its children will also be recursively deleted.
  - After the deletion process, the nodes' ID's have to be updated.

- `overrideStyle selector="<CSS_Selector>" style="<newStyle>"`
  - Overrides the `style` attribute of all the nodes which satisfy the CSS selector.
  - The new style is a string of the following form: `attributeName1:value1;attributeName2:value2;...attributeNameN:valueN;`

- `appendStyle selector="<CSS_Selector>" style="<newStyle>"`
  - Updates the style of all the nodes which satisfy the CSS selector, in the following way:
    - if the style already exists in the list, its value is modified with the new one.
    - if the style does not exist in the list, it is added at the end.
  - The new style is a string of the following form: `attributeName1:value1;attributeName2:value2;...attributeNameN:valueN;`

## CSS Selectors

The following CSS selectors are supported by the program:
- `#ID`: Looks for the tag with id=`ID`.
- `.class`: Looks for all the tags which contain the class `class`.
- `element`: Looks for all the `<element>` tags.
- `element.class`: Looks for all the `<element>` tags which contain the class `class`.
- `element1>element2`: Looks for all the `<element2>` tags which have `<element1>` as their parent.
- `element1 element2`: Looks for all the tags `<element2>` which are nested inside an `<element1>` tag.

## How to run the program?

After having completed the `input.html` and `commands.in` files following the specified rules, you can run the program using `make && make run`.
You can then see the modified HTML code inside the `out` folder, in the `commands.out` file. There is also an option to check the program
for memory leaks, using the `make valgrind` command.

## Technical Details and Source Files

A short description of each folder can be found here:

- `GenericList`: my own implementation of a generic singly linked list.
- `HtmlTree`: contains the CSS Selector and HTML Tree logic.
  - Here, in the buildTree() function, we also find the input parsing logic of the program.
  - As this assignment was submitted during my first university year, I had no knowledge of Automata Theory.
  - That being said, the buildTree() function is a rudimentary finite state machine which parses the input string character by character.
- `InOut`: contains the I/O logic, but also some string parsing capabilites
