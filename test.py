import os
import subprocess
import sys
import re

# ==========================================
# CONFIGURATION
# ==========================================
# Ta commande exacte avec les arguments de ton projet 
COMMAND = ["./a.out", "21", "500", "100", "100", "100", "5", "50", "fifo"]

C_WRAPPER = "malloc_wrapper.c"
SO_WRAPPER = "malloc_wrapper.so"
VALGRIND_LOG = "valgrind_out.txt"

# ==========================================
# LE FAUX MALLOC (Intercepteur)
# ==========================================
C_CODE = """
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static int malloc_count = 0;

void *malloc(size_t size) {
    char *env = getenv("FAIL_AT");
    if (env) {
        int fail_at = atoi(env);
        malloc_count++;
        if (malloc_count == fail_at) {
            // write() brut pour ne pas generer de malloc interne de la libc
            char msg[] = "\\n[!] INJECTION: Malloc bloque.\\n";
            write(2, msg, strlen(msg));
            return NULL;
        }
    }
    void *(*real_malloc)(size_t) = dlsym(RTLD_NEXT, "malloc");
    return real_malloc(size);
}
"""

def setup_wrapper():
    print("[*] Compilation du wrapper malloc...")
    with open(C_WRAPPER, "w") as f:
        f.write(C_CODE)
    
    # Compilation en bibliothèque partagée
    if subprocess.run(["cc", "-shared", "-fPIC", "-ldl", "-o", SO_WRAPPER, C_WRAPPER]).returncode != 0:
        print("Erreur : Impossible de compiler le wrapper.")
        sys.exit(1)

def cleanup():
    if os.path.exists(C_WRAPPER): os.remove(C_WRAPPER)
    if os.path.exists(SO_WRAPPER): os.remove(SO_WRAPPER)
    if os.path.exists(VALGRIND_LOG): os.remove(VALGRIND_LOG)

def check_leaks_in_heap_summary(valgrind_log_content):
    """
    Analyse le HEAP SUMMARY de Valgrind.
    Si 'in use at exit' est different de 0 bytes -> LEAK !
    """
    match = re.search(r"in use at exit:\s*([0-9,]+)\s+bytes", valgrind_log_content)
    if match:
        bytes_left = match.group(1).replace(",", "")
        if int(bytes_left) > 0:
            return f"{match.group(1)} bytes non liberes"
    return None

def test_mallocs():
    iteration = 1
    total_leaks = 0
    total_crashes = 0

    print(f"[*] Demarrage du Crash-Test sur : {' '.join(COMMAND)}")
    print("[*] Scan absolu du HEAP SUMMARY de Valgrind...\\n")

    while True:
        env = os.environ.copy()
        env["LD_PRELOAD"] = f"./{SO_WRAPPER}"
        env["FAIL_AT"] = str(iteration)
        env["LC_ALL"] = "C" # Force l'anglais pour éviter les problèmes de traduction de Valgrind

        valgrind_cmd = [
            "valgrind", 
            "--leak-check=full", 
            f"--log-file={VALGRIND_LOG}"
        ] + COMMAND

        result = subprocess.run(valgrind_cmd, env=env, capture_output=True, text=True)
        
        # Si notre message d'injection n'est pas apparu dans stderr, on a testé tous les mallocs
        if "[!] INJECTION" not in result.stderr:
            print(f"\\n[+] Fin : Le programme a effectue un total de {iteration - 1} mallocs.")
            break

        status = f"Test Malloc n°{iteration:<3}"

        # 1. Vérification du crash (Segfault)
        # Un comportement normal suite à un malloc qui échoue doit être un exit propre (0, 1, etc.)
        # Si le code de retour indique un signal de plantage (ex: 139 pour SIGSEGV)
        if result.returncode < 0 or result.returncode in [139, 134]:
            print(f"💥 {status} | SEGFAULT ! (Ton code ne verifie pas le retour de ce malloc)")
            total_crashes += 1
            iteration += 1
            continue

        # 2. Vérification stricte de la mémoire restante au format texte
        if os.path.exists(VALGRIND_LOG):
            with open(VALGRIND_LOG, "r") as f:
                log_content = f.read()
            
            leak_details = check_leaks_in_heap_summary(log_content)
            if leak_details:
                print(f"❌ {status} | LEAK DETECTE ! ({leak_details})")
                total_leaks += 1
            else:
                print(f"✅ {status} | CLEAN (Erreur geree + 0 octet restant)")
        else:
            print(f"⚠️  {status} | Impossible de lire le rapport Valgrind.")

        iteration += 1

    # Rapport final
    print("\\n" + "="*60)
    print("BILAN DU COMPORTEMENT EN CAS D'ECHEC ALLOCATION :")
    if total_leaks == 0 and total_crashes == 0:
        print("🏆 PERFECTION ! Ton code gere les echecs de TOUS les mallocs et libere TOUT.")
    else:
        print(f"⚠️  Total : {total_crashes} plantage(s) (Segfault) et {total_leaks} fuite(s) de memoire.")
    print("="*60)

if __name__ == "__main__":
    try:
        setup_wrapper()
        test_mallocs()
    finally:
        cleanup()