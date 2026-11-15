#!/usr/bin/env python3
"""Exercise Linux GUI installation and menu discovery without network downloads."""
import json
import os
from pathlib import Path
import shutil
import subprocess
import tarfile
import tempfile


ROOT = Path(__file__).resolve().parents[3]


def main():
    with tempfile.TemporaryDirectory(prefix="of pg ") as temporary:
        temp = Path(temporary)
        tree = temp / "of with spaces"
        shutil.copytree(ROOT / "scripts", tree / "scripts")
        wrapper = tree / "scripts/linux/download_projectGenerator.sh"
        wrapper.chmod(0o755)
        requests = temp / "requests"
        gui_marker = temp / "gui-ran"
        env = dict(os.environ, PLATFORM="linux", BITS="64", OF_ANIM="0",
                   OF_NONINTERACTIVE="1", REQUESTS=str(requests),
                   GUI_EXEC_MARKER=str(gui_marker))
        env.pop("OF_LANG", None)
        env.pop("OF_I18N_DIR", None)
        (tree / "scripts/dev/downloader.sh").write_text(
            'downloader() { printf "%s\\n" "$1" >> "$REQUESTS"; '
            'cp "$PG_FIXTURE" "${1##*/}"; }\n')
        installed = tree / "projectGenerator"
        cli_source = '#!/usr/bin/env bash\necho \'{"openFrameworks projectGenerator":"fixture-cli"}\'\n'

        def run(args, success=True):
            result = subprocess.run(args, env=env, text=True, capture_output=True,
                                    stdin=subprocess.DEVNULL, timeout=30)
            assert (result.returncode == 0) == success, result.stdout + result.stderr
            return result

        def fixture(layout="asar", with_cli=True):
            source = temp / "bundle"
            shutil.rmtree(source, ignore_errors=True)
            source.mkdir()
            (source / "projectGenerator").write_text(
                '#!/usr/bin/env bash\necho ran > "$GUI_EXEC_MARKER"\nexit 99\n')
            if layout == "asar":
                (source / "resources").mkdir()
                (source / "resources/app.asar").write_text("archive")
                cli = source / "resources/app.asar.unpacked/app/projectGenerator"
            else:
                cli = source / "resources/app/app/projectGenerator"
            cli.parent.mkdir(parents=True, exist_ok=True)
            if with_cli:
                cli.write_text(cli_source)
            archive = temp / "fixture.gz"
            with tarfile.open(archive, "w:gz") as bundle:
                bundle.add(source, arcname="projectGenerator-0.122.0-linux")
            env["PG_FIXTURE"] = str(archive)

        def version():
            result = run(["bash", str(tree / "scripts/of.sh"), "version", "pg", "--json"])
            assert not gui_marker.exists(), "Status/version launched the GUI"
            return json.loads(result.stdout)["data"]

        fixture()
        for arch, package in [("x86_64", "projectGenerator-linux-gui.gz"),
                              ("64", "projectGenerator-linux-gui.gz"),
                              ("arm64", "projectGenerator-linux-arm64-gui.gz"),
                              ("aarch64", "projectGenerator-linux-arm64-gui.gz")]:
            run(["bash", str(tree / "scripts/dev/download_pg.sh"),
                 "-p", "linux", "-a", arch, "-t", "fixture-tag"])
            assert requests.read_text().splitlines()[-1].endswith("/fixture-tag/" + package)
            for binary in ["projectGenerator", "projectGeneratorCmd",
                           "resources/app.asar.unpacked/app/projectGenerator"]:
                assert os.access(installed / binary, os.X_OK), binary
            assert version()["version"] == "fixture-cli"
        print("PASS x64/ARM64 GUI packages, architecture aliases, executables and CLI version")

        (installed / "projectGeneratorCmd").unlink()
        assert version()["path"].endswith("resources/app.asar.unpacked/app/projectGenerator")
        (installed / "resources/app.asar.unpacked/app/projectGenerator").unlink()
        assert version()["installed"] is False
        print("PASS bundled ASAR CLI discovery; GUI-only install never runs as CLI")

        fixture("legacy")
        run(["bash", str(wrapper), "-t", "legacy-tag"])
        (installed / "projectGeneratorCmd").unlink()
        assert version()["path"].endswith("resources/app/app/projectGenerator")
        print("PASS older unpacked GUI layout and wrapper arguments with spaces in checkout path")

        sentinel = installed / "preserve"
        sentinel.write_text("existing install")
        fixture(with_cli=False)
        args = ["bash", str(tree / "scripts/dev/download_pg.sh"), "-p", "linux", "-a", "64"]
        run(args, success=False)
        assert sentinel.read_text() == "existing install"
        (temp / "fixture.gz").write_text("invalid archive")
        run(args, success=False)
        assert sentinel.exists()
        assert not list(tree.glob("projectGenerator.*")), "Staging directory leaked"
        before = requests.read_text()
        run(args[:-1] + ["armv7l"], success=False)
        assert requests.read_text() == before and sentinel.exists()
        print("PASS incomplete/corrupt packages and unsupported CPUs preserve the installed generator")

        fixture()
        shutil.rmtree(installed)
        installed.mkdir()
        cli = installed / "projectGenerator"
        cli.write_text(cli_source)
        cli.chmod(0o755)
        before = requests.read_text()
        command = ["bash", str(tree / "scripts/of.sh"), "update", "pg"]
        run(command)
        assert requests.read_text() != before, "Menu skipped the CLI-only install"
        assert (installed / "resources/app.asar").exists()
        assert version()["version"] == "fixture-cli"
        before = requests.read_text()
        run(command)
        assert requests.read_text() == before, "Complete install downloaded again"
        result = run(["bash", str(tree / "scripts/of.sh"), "version", "--json"])
        assert json.loads(result.stdout)["data"]["cli"] == "0.7.1"
        print("PASS of update pg upgrades CLI-only installs, skips complete installs, reports 0.7.1")


if __name__ == "__main__":
    main()
