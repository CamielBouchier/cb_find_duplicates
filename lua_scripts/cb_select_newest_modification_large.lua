-------------------------------------------------------------------------------
--
-- $BeginLicense$
-- $EndLicense$
--
-------------------------------------------------------------------------------

title       = "Newest (modification time) > 10MB"
description = "Select newest  (modification time) from group, but leave < 10MB untouched."

function select(files, mtimes, ctimes, inodes, size)
  -- files : table of filenames.
  -- mtimes : corresponding table of modification times. 
  -- ctimes : corresponding table of creation times. 
  -- inodes : corresponding table of (fake) inodes. 
  -- size  : filesize.
  local selected = {}
  pivot = 1
  for i=2, #files do
    if mtimes[i] < mtimes[pivot] then
      pivot = i
    end
  end
  for i=1, #files do
    selected[i] = (i ~= pivot) and (size > 10485760)
  end
  return selected
end

-------------------------------------------------------------------------------
