import cc.openframeworks.OFZipUtil;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

public class OFZipUtilTest
{
  private static byte[] zip(String name, String contents, boolean directory) throws Exception
  {
    ByteArrayOutputStream bytes = new ByteArrayOutputStream();
    try (ZipOutputStream zip = new ZipOutputStream(bytes))
    {
      ZipEntry entry = new ZipEntry(name);
      zip.putNextEntry(entry);
      if (!directory)
        zip.write(contents.getBytes(StandardCharsets.UTF_8));
      zip.closeEntry();
    }
    return bytes.toByteArray();
  }

  private static void extract(File output, String name, String contents) throws Exception
  {
    OFZipUtil.extract(new ByteArrayInputStream(zip(name, contents, false)), output);
  }

  private static void assertMissing(Path path, String message)
  {
    if (Files.exists(path))
      throw new AssertionError(message + ": " + path);
  }

  public static void main(String[] args) throws Exception
  {
    Path parent = Files.createTempDirectory("ofziputil-test-");
    File output = Files.createDirectory(parent.resolve("output")).toFile();

    extract(output, "nested/good.txt", "good");
    Path good = output.toPath().resolve("nested/good.txt");
    if (!Files.isRegularFile(good)
        || !"good".equals(Files.readString(good, StandardCharsets.UTF_8)))
      throw new AssertionError("A safe nested entry was not extracted correctly");

    Path escaped = parent.resolve("escaped.txt");
    extract(output, "../escaped.txt", "bad");
    assertMissing(escaped, "A parent traversal entry escaped the output directory");

    extract(output, "nested/../../escaped.txt", "bad");
    assertMissing(escaped, "A nested parent traversal entry escaped the output directory");

    Path absolute = parent.resolve("absolute.txt");
    extract(output, absolute.toString(), "bad");
    assertMissing(absolute, "An absolute entry escaped the output directory");

    Path escapedDirectory = parent.resolve("escaped-directory");
    OFZipUtil.extract(
        new ByteArrayInputStream(zip("../escaped-directory/", "", true)), output);
    assertMissing(escapedDirectory, "A directory entry escaped the output directory");

    Path outside = Files.createDirectory(parent.resolve("outside"));
    Files.createSymbolicLink(output.toPath().resolve("link"), outside);
    extract(output, "link/escaped.txt", "bad");
    assertMissing(outside.resolve("escaped.txt"),
        "An entry escaped through a symbolic link in the output directory");

    System.out.println("OFZipUtil security regression tests passed");
  }
}
