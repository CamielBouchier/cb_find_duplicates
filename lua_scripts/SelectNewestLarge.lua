-------------------------------------------------------------------------------
--
-- $BeginLicense$
-- $EndLicense$
--
-------------------------------------------------------------------------------

Title       = "Newest > 10MB"
Description = "Select newest from group, but leave < 10MB untouched."

function Select(Files,Times,Size)
  -- Files : table of filenames.
  -- Times : corresponding table of modification times. 
  -- Size  : filesize.
  local Selected = {}
  Pivot = 1
  for i=2,#Files do
    if Times[i] < Times[Pivot] then
      Pivot = i
    end
  end
  for i=1,#Files do
    Selected[i] = (i ~= Pivot) and (Size > 10485760)
  end
  return Selected
end

-------------------------------------------------------------------------------
