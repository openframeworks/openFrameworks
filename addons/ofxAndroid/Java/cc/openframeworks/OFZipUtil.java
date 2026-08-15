package cc.openframeworks;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import android.util.Log;
import androidx.annotation.Keep;

@Keep
public class OFZipUtil
{
  private static final int  BUFFER_SIZE = 4096;

  private static File resolveEntry(File outdir, String name) throws IOException
  {
    File canonicalOutdir = outdir.getCanonicalFile();
    File destination = new File(canonicalOutdir, name).getCanonicalFile();
    String outdirPath = canonicalOutdir.getPath();
    String destinationPath = destination.getPath();

    if (!destinationPath.equals(outdirPath)
        && !destinationPath.startsWith(outdirPath + File.separator))
      throw new IOException("Zip entry is outside the target directory: " + name);

    return destination;
  }

  public static void extractFile(ZipInputStream in, File outdir, String name) throws IOException
  {
    byte[] buffer = new byte[BUFFER_SIZE];
    File destination = resolveEntry(outdir, name);
    try (BufferedOutputStream out = new BufferedOutputStream(new FileOutputStream(destination)))
    {
      int count;
      while ((count = in.read(buffer)) != -1)
        out.write(buffer, 0, count);
    }
  }

  private static void createDirectories(File outdir, String path) throws IOException
  {
    File directory = resolveEntry(outdir, path);
    if (!directory.exists() && !directory.mkdirs() && !directory.isDirectory())
      throw new IOException("Could not create directory: " + directory);
  }

  public static void mkdirs(File outdir,String path)
  {
    try
    {
      createDirectories(outdir, path);
    }
    catch (IOException e)
    {
      throw new IllegalArgumentException(e);
    }
  }

  public static String dirpart(String name)
  {
    int s = name.lastIndexOf( File.separatorChar );
    return s == -1 ? null : name.substring( 0, s );
  }

  /***
   * Extract zipfile to outdir with complete directory structure
   * @param zipfile Input .zip file
   * @param outdir Output directory
   */
  public static void extract(InputStream zipfile, File outdir)
  {
    try
    {
      ZipInputStream zin = new ZipInputStream(zipfile);
      ZipEntry entry;
      String name, dir;
      Log.i("OF", "uncompressinggggg ");
      while ((entry = zin.getNextEntry()) != null)
      {
        name = entry.getName();
        // Validate every entry before creating directories or opening files.
        resolveEntry(outdir, name);
        if( entry.isDirectory() )
        {
          createDirectories(outdir,name);
          continue;
        }
        /* this part is necessary because file entry can come before
         * directory entry where is file located
         * i.e.:
         *   /foo/foo.txt
         *   /foo/
         */
        dir = dirpart(name);
        if( dir != null )
          createDirectories(outdir,dir);

        extractFile(zin, outdir, name);
      }
      zin.close();
    } 
    catch (IOException e)
    {
      e.printStackTrace();
    }
  }
}
