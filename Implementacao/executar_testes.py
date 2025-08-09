import subprocess
import os

# Caminhos base
dir_construir = r".\Entradas Árvores\Construir"
dir_consultar = r".\Entradas Árvores\Consultar"
dir_saida = r".\Resultados"
exe_path = r".\ABB.exe"

# Tamanhos de teste
tamanhos = [50, 100, 200, 300, 500, 750, 1000, 1500, 2000, 3000, 5000, 7500, 10000, 12500, 15000, 15000, 20000, 25000, 30000, 40000, 50000, 75000, 100000, 125000, 150000, 175000, 200000, 225000, 250000]  # Adicione mais tamanhos conforme necessário

# Cria pasta de saída se não existir
os.makedirs(dir_saida, exist_ok=True)

# Loop sobre tamanhos
for tamanho in tamanhos:
    construir_file = os.path.join(dir_construir, f"{tamanho}.txt")
    consultar_file = os.path.join(dir_consultar, f"{tamanho}.txt")
    saida_txt = os.path.join(dir_saida, f"resultado_{tamanho}.txt")

    print(f"\n=== Executando para {tamanho} elementos ===")

    with open(saida_txt, "w", encoding="utf-8") as f_saida:
        f_saida.write(f"##### RESULTADO PARA {tamanho} ELEMENTOS #####\n\n")

        for i in range(1, 6):
            print(f"  → Execução {i}...")

            comando = [
                exe_path,
                construir_file,
                consultar_file,
                str(tamanho),
                "3"
            ]

            try:
                resultado = subprocess.run(
                    comando,
                    capture_output=True,
                    text=True,
                    shell=True
                )

                f_saida.write(f"\n=== Execução {i} ===\n")
                f_saida.write(resultado.stdout)
                f_saida.write(resultado.stderr)
                f_saida.write("\n")

            except Exception as e:
                f_saida.write(f"\nErro na execução {i}: {e}\n")
