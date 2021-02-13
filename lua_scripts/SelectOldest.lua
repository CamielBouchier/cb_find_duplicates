-------------------------------------------------------------------------------
--
-- $BeginLicense$
-- $EndLicense$
--
-------------------------------------------------------------------------------

Title       = "Oldest"
Description = "Select oldest from group"

function Select(Files,Times,Size)
  -- Files : table of filenames.
  -- Times : corresponding table of modification times. 
  -- Size  : filesize.
  local Selected = {}
  Pivot = 1
  for i=2,#Files do
    if Times[i] > Times[Pivot] then
      Pivot = i
    end
  end
  for i=1,#Files do
    Selected[i] = (i ~= Pivot)
  end
  return Selected
end

-------------------------------------------------------------------------------
