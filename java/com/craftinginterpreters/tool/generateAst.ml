if Array.length Sys.argv <> 2 then print_endline "Usage: generate_ast <output directory>"
else Printf.printf "path to generate: %s\n%!" Sys.argv.(1);
    


let defineAst outputDir baseName types = 
  print_endline outputDir;
  print_endline baseName;
  for i = 0 to Array.length types - 1 do
    print_endline types.(i)
  done

defineAst Sys.argv.(1) "Expr" [
  "Assign   : Token name, Expr value";
  "Binary   : Expr left, Token operator, Expr right";
  "Grouping : Expr expression";
  "Literal  : Object value";
  "Unary    : Token operator, Expr right";
  "Variable : Token name"
]